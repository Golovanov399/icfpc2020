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
		cerr << "Bad server url" << endl;
		return 1;
	}
	const string serverName = urlMatches[1];
	const int serverPort = stoi(urlMatches[2]);
	httplib::Client client(serverName, serverPort);
	const shared_ptr<httplib::Response> serverResponse = 
		client.Get((serverUrl + "?playerKey=" + playerKey).c_str());

	if (!serverResponse) {
		cerr << "No response from server" << endl;
		return 2;
	}
	
	if (serverResponse->status != 200) {
		cerr << "Server returned error: " <<
			httplib::detail::status_message(serverResponse->status) << 
			" (" << serverResponse->status << ")" << endl;
		return 3;
	}

	return 0;
}

