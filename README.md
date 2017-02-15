# Priority based pre-threaded image processing server
# Each image in the client folder acts as an individual client requesting a server process.
# All the requests are sent to the server at the same time, this determines the server's capability to set priority to the individual       clients and process them in a queue.
# All the requests are services by the server using multiple threads for faster processing time.
