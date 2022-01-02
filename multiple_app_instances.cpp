#include "App.h"
#include "main.h"

/* Note that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support */

int multiple_app_instances() {

	/* Overly simple hello world app */
	uWS::SSLApp app = uWS::SSLApp({
	  .key_file_name = "../misc/key.pem",
	  .cert_file_name = "../misc/cert.pem",
	  .passphrase = "1234"

		});

	/* Overly simple hello world app */
	uWS::SSLApp app2 = uWS::SSLApp({
	  .key_file_name = "../misc/key.pem",
	  .cert_file_name = "../misc/cert.pem",
	  .passphrase = "1234"
	  
		});

	/* ws->getUserData returns one of these */
	struct PerSocketData {
		std::string data;
	};

	PerSocketData ps;
	ps.data = "Hello";

	/* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
	 * You may swap to using uWS:App() if you don't need SSL */
	uWS::App app3 = uWS::App().ws<PerSocketData>("/*", {
		/* Settings */
		.compression = uWS::SHARED_COMPRESSOR,
		.maxPayloadLength = 16 * 1024 * 1024,
		.idleTimeout = 16,
		.maxBackpressure = 1 * 1024 * 1024,
		.closeOnBackpressureLimit = false,
		.resetIdleTimeoutOnSend = false,
		.sendPingsAutomatically = true,
		/* Handlers */
		.upgrade = nullptr,
		.open = [](auto* ws) {
			auto d = ws->getUserData();
			std::cout << d->data << "\n";
			/* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
			ws->subscribe("broadcast");
		},
		.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << message << "\n";
			auto d = ws->getUserData();
			d->data = message;
		},
		.drain = [](auto*/*ws*/) {
			/* Check ws->getBufferedAmount() here */
		},
		.ping = [](auto*/*ws*/, std::string_view) {
			/* Not implemented yet */
		},
		.pong = [](auto*/*ws*/, std::string_view) {
			/* Not implemented yet */
		},
		.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
			/* You may access ws->getUserData() here */
		}
		}).listen(9093, [](auto* listen_socket) {
			if (listen_socket) {
				std::cout << "Listening on port " << 9093 << std::endl;
			}
			});


		app.get("/", [](auto* res, auto*/*req*/)
			{
				res->end("Hello world!");
			}
		);
		app.get("/hello", [](auto* res, uWS::HttpRequest* req)
			{
				std::cout << req->getQuery() << "\n";
				res->end("Hello world 11111!");
			}
		);
		app.get("/hello2", [](auto* res, uWS::HttpRequest* req)
			{
				std::cout << req->getQuery() << "\n";
				res->end("Hello world 22222!");
			}
		);
		app.get("/hello3", [](auto* res, uWS::HttpRequest* req)
			{
				std::cout << req->getQuery() << "\n";
				res->end("Hello world 333333!");
			}
		);

		app.listen(3000, [](auto* listen_socket)
			{
				if (listen_socket) {
					std::cout << "Listening on port " << 3000 << std::endl;
				}
			}
		);

		app2.get("/", [](auto* res, auto*/*req*/)
			{
				res->end("Hello world!");
			}
		);

		app2.listen(3001, [](auto* listen_socket)
			{
				if (listen_socket) {
					std::cout << "Listening on port " << 3001 << std::endl;
				}
			}
		);
		app2.run();
		std::cout << "Failed to listen on port 3001" << std::endl;
}