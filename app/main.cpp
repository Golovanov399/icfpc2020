#include <bits/stdc++.h>
#include "httplib.h"

using namespace std;

int main(int argc, char* argv[])
{
  const string serverUrl(argv[1]);
  const string playerKey(argv[2]);

  cout << "ServerUrl: " << serverUrl << "; PlayerKey: " << playerKey << endl;

  const regex urlRegexp("http://(.+):(\\d+)");
  smatch urlMatches;
  if (!regex_search(serverUrl, urlMatches, urlRegexp) || urlMatches.size() != 3) {
    cout << "Unexpected server response:\nBad server URL" << endl;
    return 1;
  }
  const string serverName = urlMatches[1];
  const int serverPort = stoi(urlMatches[2]);
  httplib::Client client(serverName, serverPort);

  auto send = [&](std::string message) {
    cout << "send " << message << " to server" << endl;

    const shared_ptr<httplib::Response> serverResponse =
        client.Post(serverUrl.c_str(), message.c_str(), "text/plain");

    if (!serverResponse) {
      cout << "Unexpected server response:\nNo response from server" << endl;
      return 1;
    }

    if (serverResponse->status != 200) {
      cout << "Unexpected server response:\nHTTP code: " << serverResponse->status
          << "\nResponse body: " << serverResponse->body << endl;
      return 2;
    }

    cout << "Server response: " << serverResponse->body << endl;
    return 0;
  };

  send(playerKey);
  for (int len = 1; len < 1000; ++len) {
    send(std::string('1', len));
  }

  return 0;
}

