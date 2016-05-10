all: client server

client:
	$(MAKE) -C chat_client
	cp chat_client/client client

server:
	$(MAKE) -C chat_server
	cp chat_server/server server

clean:
	$(MAKE) -C chat_client clean
	$(MAKE) -C chat_server clean
	$(MAKE) -C common clean
	rm -f client server

.PHONY: all client server clean