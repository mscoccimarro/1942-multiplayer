#include "../libs/socket/sock_dep.h" /* socket dependencies */
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/GameConf.h"
#include "../xml/parser/GameParser.h"
#include "../xml/conf/ClientConf.h"
#include "../libs/mensaje/mensaje.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <time.h>
#define DEBUG 1
#include "../libs/debug/dg_msg.h"

#include "../utils/K.h"

using namespace std;

int gfd = 0;
bool connected = false;
ClientConf* cc;
Logger* logger = Logger::instance();
mutex theMutex;
Menu clientMenu("Menu de opciones del Cliente");

const int MAX_UNREACHABLE_TIME = 5;
bool recibi;

void checkAliveSend(int sfd) {
  char buf[1] = { '1' };

  while(true) {
    if(!connected) return;
    // 4s timed send
    usleep(4000000);
    send(sfd, &buf, 1, 0);
  }
}

void closeConnection() {
  close(gfd);
  connected = false;
  logger->warn(CONNECTION_CLOSE);
  DEBUG_WARN(CONNECTION_CLOSE);
}

void receiving(int sfd, const int MAX_DATA_SIZE, const char *IP){
  int numBytesRead;
  timeval timeout;
  timeout.tv_sec = MAX_UNREACHABLE_TIME;
  timeout.tv_usec = 0;
  Mensaje* buf = new Mensaje;

  while (connected) {
    // seteo el timeout de recepcion de mensajes
    if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)) < 0) {
      cout << "Error sockopt" << endl;
      exit(1);
    }

    if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
      close(gfd);
      connected = false;
      logger->warn(CONNECTION_TIMEOUT);
      DEBUG_WARN(CONNECTION_TIMEOUT);
      return;
    }

    if (numBytesRead > 0) {
      if(numBytesRead != 1) {	
	string recvMsg = string(buf->valor);
	logger->info(SERVER_MSG(recvMsg));
	DEBUG_PRINT(SERVER_MSG(recvMsg));
	recibi = true;
      }
    }
    if (numBytesRead == 0){
      logger->warn(CONNECTION_LOST);
      DEBUG_WARN(CONNECTION_LOST);
      connected = false;
      close(sfd);
    }
  }
}

void srvConnect() {
  if (connected) {
    logger->warn(CONNECTION_ACTIVE);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_ACTIVE);
    theMutex.unlock();
    return;
  }

  const int MAX_DATA_SIZE = 10000; /* Max. number of bytes for recv */
  int sfd, numBytesRead; /* socketFD, bytes read count */
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
  string serverIP = cc->getServerIP();

  /* Create socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    logger->error(SOCKET_ERROR);
    exit(-1);
  }

  gfd = sfd;

  server.sin_family = AF_INET;
  server.sin_port = htons(cc->getServerPort());
  if ((inet_aton(serverIP.c_str(), &server.sin_addr)) == 0) {
    logger->error("IP invalido");
    exit(-1);
  }

  bzero(&(server.sin_zero), 8);

  /* Connect to server */
  short triesLeft = 3;

  while (!connected && triesLeft) {
    if (connect(sfd, (struct sockaddr*) &server, sizeof(struct sockaddr))
	== -1) {
      triesLeft--;
      /* 5s delay for retry */
      if (triesLeft) {
	logger->error(CONNECTION_RETRY);
	theMutex.lock();
	DEBUG_WARN(CONNECTION_RETRY);
	theMutex.unlock();
	usleep(5000000);
      } else {
	logger->warn(CONNECTION_ERROR(serverIP));
	theMutex.lock();
	DEBUG_PRINT(CONNECTION_ERROR(serverIP));
	theMutex.unlock();
	return;
      }
    } else {
      connected = true;
    }
  }

  timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 5000;

  // seteo el timeout de recepcion de mensajes
  if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
	sizeof(timeout)) < 0) {
    cout << "Error sockopt" << endl;
    exit(1);
  }

  // Get server welcome message
  if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
    logger->warn(CONNECTION_REJECTED);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_REJECTED);
    theMutex.unlock();
    connected = false;
    close(sfd);
  }
  if (numBytesRead>0) {
    logger->info(CONNECTION_SUCCESS(serverIP));
    theMutex.lock();
    DEBUG_PRINT(CONNECTION_SUCCESS(serverIP));
    buf[numBytesRead] = '\0';
    string recvMsg = string(buf);
    logger->info(SERVER_MSG(recvMsg));
    DEBUG_PRINT(SERVER_MSG(recvMsg));
    theMutex.unlock();
  }
  if (numBytesRead == 0){
    logger->warn(CONNECTION_LOST);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_LOST);
    theMutex.unlock();
    connected = false;
    close(sfd);
  }

  // Create thread for receiving data from server
  if (connected){
    thread tCheckAliveSend(checkAliveSend, sfd);
    tCheckAliveSend.detach();

    thread tReceiving(receiving, sfd, MAX_DATA_SIZE, serverIP.c_str());
    tReceiving.detach();
  }
}

void srvDisconnect() {
  if (connected) {
    closeConnection();
  } else {
    logger->warn(CONNECTION_NOT_ACTIVE);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_NOT_ACTIVE);
    theMutex.unlock();
  }
}

void limpiarMemoria() {
  //TODO: Pierde memoria por el logger y el Parser. No pueden ser borrados. Chequearlo
  //	delete logger;
  //	delete cc;
}

void exitPgm() {
  if(connected)
    closeConnection();
  logger->warn(CLIENT_CLOSE);
  theMutex.lock();
  DEBUG_WARN(CLIENT_CLOSE);
  theMutex.unlock();
  limpiarMemoria();
  exit(0);
}

bool sendData(Mensaje* data, int dataLength) {
  recibi = false;
  if(send(gfd, data, dataLength, 0) == -1) {
    logger->error(SEND_FAIL);
    //    theMutex.lock();
    DEBUG_WARN(SEND_FAIL);
    //    theMutex.unlock();
    return false;
  }
  
  return true;
}

bool sendMsg(string id) {
  if(!connected) {
    logger->warn(SEND_CERROR);
    //  theMutex.lock();
    DEBUG_WARN(SEND_CERROR);
    // theMutex.unlock();
    return false;
  }

  Mensaje* msgToSend;

  for(int i = 0; i < cc->getMessages().size(); i++) {
    string msgID = cc->getMessages()[i]->id;
    if(msgID == id) {
      msgToSend = cc->getMessages()[i];
      break;
    }
  }

  int dataLength = sizeof(Mensaje);
  logger->info(SENT_DATA(msgToSend->valor));
  // theMutex.lock();
  DEBUG_PRINT(SENT_DATA(msgToSend->valor));
  // theMutex.unlock();
  return sendData(msgToSend, dataLength);
}

void addMsgOptions() {
  for (int i = 0; i < cc->getMessages().size(); i++) {
    string msgID = string(cc->getMessages()[i]->id);
    string optionName = "Enviar mensaje " + msgID;
    clientMenu.addOption(optionName, sendMsg, msgID);
  }
}

void cycle() {
  if (!connected){
    logger->warn(CONNECTION_NOT_ACTIVE);
    DEBUG_WARN(CONNECTION_NOT_ACTIVE);
    return;
  }

  double timeout = 0;
  cout << "Ingrese duracion (en milisegundos): ";
  cin >> timeout;

  while (timeout <= 0) {
    cout << endl << "Error - Debe ingresar un numero mayor a cero" << endl;
    cout << "Ingrese nuevamente durancion (en milisegundos): ";
    cin >> timeout;
  }
  timeout = timeout * 1000;

  logger->info("Se corre ciclar en " + to_string(timeout) + " milisegundos.");
  double diferencia = 0;
  int i = 0;
  recibi = true;
  clock_t start = clock();
  while (diferencia <= timeout && connected) {
    if (recibi){
      if (i >= cc->getMessages().size())
	i = 0;
      //cout << endl << "En el i: " << i;

      if(!sendMsg(cc->getMessages()[i]->id))
	return;

      i++;
    }
    diferencia = clock() - start;
  }
  usleep(10000); // solo para que el ultimo se muestre antes de hacer display del menu
}

int main(int argc, char* argv[]) {
  const char* fileName = argv[1] ? argv[1] : "default-cc.xml";
  GameConf* gc = GameParser::parse(fileName);

  //TODO: Cuando se use el file name hay que utilizar el delete, para que pierda menos memoria
  //  delete fileName;

  //  cout << cc->getMessages()[0];

  clientMenu.addOption("Conectar", srvConnect);
  clientMenu.addOption("Desconectar", srvDisconnect);
  addMsgOptions();
  clientMenu.addOption("Ciclar", cycle);
  clientMenu.addOption("Salir", exitPgm);

  clientMenu.display();

  return 0;
}
