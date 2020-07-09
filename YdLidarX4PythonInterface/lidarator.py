# -*- coding: utf-8 -*-
#!/usr/bin/python

from colors import colors

import time
import argparse
import threading
import signal
import sys
import os
import multiprocessing
import queue
import json
import pickle
import socket
import uuid
import hashlib
import os
import base64
from textwrap import wrap

import PyLidar3

log = 0;

commsock = 0
client_sock = 0

lidar = 0

BUFF_SIZE = 1024
PORT = "/dev/ttyUSBLidar"

# Handler de la señal Ctrl + C

def ctrlc_exit(sig, frame):

	if log==1:
		print(threadwarning,'Cerrando...')

	try:
		commsock.shutdown(socket.SHUT_RDWR)
	except:
		pass
	commsock.close()

	if type(commsock)==type(client_sock):
		try:
			client_sock.shutdown(socket.SHUT_RDWR)
		except:
			pass
		client_sock.close()
	
	if log==1:
		print(threadend,'Cierre completado')
	os._exit(1)


# Handler de la señal Ctrl + Z

def ctrlz_exit(sig, frame):

	signal.signal(signal.SIGINT, ctrlc_exit)

	if log==1:
		print(threadwarning,'Cerrando...')

	try:
		commsock.shutdown(socket.SHUT_RDWR)
	except:
		pass
	commsock.close()

	if type(commsock)==type(client_sock):
		try:
			client_sock.shutdown(socket.SHUT_RDWR)
		except:
			pass
		client_sock.close()
	
	if log==1:
		print(threadend,'Cierre completado')
	os._exit(1)

if __name__ == "__main__":

	threadlog = colors.fg.violet+threading.currentThread().getName()+" LOG:"+colors.end
	threadsuccess = colors.fg.green+threading.currentThread().getName()+" SUCCESS:"+colors.end
	threaderror = colors.fg.red2+threading.currentThread().getName()+" ERROR:"+colors.end
	threadcritical = colors.fg.red+threading.currentThread().getName()+" CRITICAL ERROR:"+colors.end
	threadcomm = colors.fg.grey+threading.currentThread().getName()+" COMUNICACION:"+colors.end
	threadend = colors.fg.blue2+threading.currentThread().getName()+" EXIT:"+colors.end
	threadwarning = colors.fg.yellow2+threading.currentThread().getName()+" WARNING:"+colors.end

	if len(sys.argv)>1:
		if sys.argv[1] == "--log":
			log = 1


	if log==1:
		print(threadlog, "Cargando...")

	signal.signal(signal.SIGINT, ctrlc_exit)

	if os.name == 'nt':
		pass
	else:
		signal.signal(signal.SIGTSTP, ctrlz_exit)

	if log==1:
		print(threadlog, "Iniciando servidor...")


	class Command:

		CMD_CONNECT, CMD_START_SCANNING, CMD_GET_DEVICE_INFO, CMD_DISCONNECT, CMD_RESET, CMD_GET_HEALTH_STATUS, CMD_GET_SAMPLE_DATA, CMD_STOP_SCANNING = (1000, 2000, 3000, 4000, 5000, 6000,7000, 8000)

	class Response:

		RES_OK, RES_FAIL, HEALTH_OK, HEALTH_BAD = (1001, 1002,2001, 2002)

	
	commsock = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)

	server_address = "0.0.0.0"

	server_port = 25138

	server = (server_address, server_port)

	try:

		commsock.bind(server)
		commsock.listen(1);

	except socket.error as e:

		if log==1:
			print(threadcritical,"Error abriendo conexion en el puerto",server_port,e)
		ctrlc_exit(0,0)

	if log==1:
		print(threadsuccess, "Servidor iniciado")

	while getattr(threading.currentThread(), "server", 1):

		if log==1:
			print(threadlog,"Esperando conexiones entrantes en ",server_address,":",server_port)

		try:

			client_sock, client = commsock.accept()

			client_address = client[0]
			client_port = client[1]

			client_sock.setblocking(0)
			client_sock.settimeout(0.01)

		except socket.error as e:

			if log==1:
				print(threaderror,"Error aceptando la conexion, reiniciando servidor...")

			continue

		else:

			if log==1:
				print(threadlog,"Conexion aceptada del cliente ",str(client))

		# Crear almacen de datos

		storedata = {}
		
		while getattr(threading.currentThread(), "server", 1):

			try:
				data = client_sock.recv(BUFF_SIZE)

				if not data.decode():
					raise Exception('No readable')

			except socket.timeout:

				time.sleep(0.05)

				pass

			except Exception as e:

				if log==1:
					print(threadwarning,"La conexion se ha cerrado inesperadamente, reiniciando servidor y desconectando lidar...")

				try:
					lidar.Disconnect()
				except:
					pass

				storedata = {}

				client_sock.close()
				client_sock = 0

				break

			else:

				if data.decode():
					if log==1:
						print(threadlog,"Se han recibido datos:",repr(data.decode()))

					# Crear response

					response = {}
					response['response'] = None
					response['health'] = None
					response['deviceinfo'] = None
					response['sampledata'] = None

					try:

						query = json.loads(data.decode())

					except:

						if log==1:
							print(threaderror,"Datos recibidos inválidos.")

						continue

					# Comprobar campos

					if not "command" in query:

						if log==1:
							print(threaderror,"Faltan parámetros")

						continue

					# Leer comandos, configurar lidar y preparar respuesta

					if query['command'] == Command.CMD_CONNECT:

						if log==1:
							print(threadcomm,str(client)," >> Conectar a lidar")
						
						storedata['lidar'] = PyLidar3.YdLidarX4(PORT,6000)

						if storedata['lidar'].Connect():
							response['response'] = Response.RES_OK
						else:
							response['response'] = Response.RES_FAIL

					if query['command'] == Command.CMD_START_SCANNING:

						if log==1:
							print(threadcomm,str(client)," >> Iniciar escaneo")

							try:

								storedata['lidargen'] = storedata['lidar'].StartScanning()

							except Exception as e:

								print(threaderror,"No se puede iniciar el escaneo",e)
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

					if query['command'] == Command.CMD_STOP_SCANNING:

						if log==1:
							print(threadcomm,str(client)," >> Detener escaneo")

							try:

								storedata['lidar'].StopScanning()

								del(storedata['lidargen'])

							except Exception as e:

								print(threaderror,"No se puede detener el escaneo")
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

					if query['command'] == Command.CMD_GET_SAMPLE_DATA:

						if log==1:
							print(threadcomm,str(client)," >> Solicitar datos de escaneo")

							try:

								sampledata = next(storedata['lidargen'])

								response['sampledata'] = [None] * 360

								for key, value in sampledata.items():
									response['sampledata'][int(key)] = value
    							

								#response['sampledata'] = sampledata
								response['response'] = Response.RES_OK

							except Exception as e:

								print(threaderror, "No se puede obtener la siguiente muestra",e);
								response['response'] = Response.RES_FAIL

					if query['command'] == Command.CMD_DISCONNECT:

						if log==1:
							print(threadcomm,str(client)," >> Desconectar dispositivo")

							try:

								storedata['lidar'].Disconnect()

							except Exception as e:

								print("TEPILLEE")
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

								try:
									del storedata['lidargen']
								except:
									pass

					if query['command'] == Command.CMD_GET_DEVICE_INFO:

						if log==1:
							print(threadcomm,str(client)," >> Solicitar informacion dispositivo")

							try:

								response['deviceinfo'] = storedata['lidar'].GetDeviceInfo()

							except Exception as e:

								print(threaderror,"El dispositivo no está conectado")
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

					if query['command'] == Command.CMD_RESET:

						if log==1:
							print(threadcomm,str(client)," >> Reiniciar dispositivo")

							try:

								storedata['lidar'].Reset()

							except Exception as e:

								print(threaderror,"El dispositivo no está conectado",e)
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

					if query['command'] == Command.CMD_GET_HEALTH_STATUS:

						if log==1:
							print(threadcomm,str(client)," >> Obtener estado de salud")

							try:

								if storedata['lidar'].GetHealthStatus():

									response['health'] = Response.HEALTH_OK

								else:

									response['health'] = Response.HEALTH_BAD

							except Exception as e:

								print(threaderror,"El dispositivo no está conectado",e)
								response['response'] = Response.RES_FAIL

							else:
								response['response'] = Response.RES_OK

					# Codificar respuesta y enviar

					if query['command'] in [Command.CMD_CONNECT, Command.CMD_START_SCANNING, Command.CMD_GET_DEVICE_INFO, Command.CMD_DISCONNECT, Command.CMD_RESET, Command.CMD_GET_HEALTH_STATUS, Command.CMD_GET_SAMPLE_DATA, Command.CMD_STOP_SCANNING]:

						response_string = json.dumps(response)

						if log==1:
							print(threadcomm,"Enviando respuesta ", response_string,">>",str(client))

						client_sock.send(response_string.encode())

					if query['command'] in [Command.CMD_DISCONNECT]:

						print(threadcomm,"Se ha solicitado cerrar la conexion...")

						try:
							client_sock.shutdown(socket.SHUT_RDWR)
						except:
							pass
						
						client_sock.close()

						print(threadsuccess,"Comunicacion cerrada.")

						break


		else:

			continue


				#break;
		# except Exception as e:

		# 	exc_type, exc_obj, exc_tb = sys.exc_info()

		# 	print(threadcritical,"Error en el servidor:",e,str(e))
		# 	fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
		# 	print(exc_type, fname, exc_tb.tb_lineno)

		# 	client_sock.shutdown(socket.SHUT_RDWR)
		# 	client_sock.close()
		# 	commsock.shutdown(socket.SHUT_RDWR)
		# 	commsock.close()

				
		# finally:
		# 	client_sock.shutdown(socket.SHUT_RDWR)
		# 	client_sock.close()
		# 	commsock.shutdown(socket.SHUT_RDWR)
		# 	commsock.close()
