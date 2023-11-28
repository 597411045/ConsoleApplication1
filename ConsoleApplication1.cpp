#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "jsoncpp.lib")

#define BUILDING_LIBCURL  
#define HTTP_ONLY         
#define CURL_STATICLIB    

#include <curl/curl.h>
#include <json/json.h>
#include <string>
#include <iostream>

using namespace std;


inline size_t onWriteData(void* buffer, size_t size, size_t nmemb, void* userp)
{
	std::string* str = dynamic_cast<std::string*>((std::string*)userp);
	str->append((char*)buffer, size * nmemb);
	return nmemb;
}


string GetToken() {
	CURL* curl = curl_easy_init();
	CURLcode res;

	string resultHeader;
	string resultBody;

	string client_id = "ioj5ilN0O9IWrbf9GDS6zmqG";
	string client_secret = "Z1CCaX4jjx05Fy7KDQPRRopbSEXaNOKd";
	string url = "https://aip.baidubce.com/oauth/2.0/token?client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
	const char* data = "";




	curl_slist* headers = nullptr;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &resultHeader);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	cout << resultHeader << endl;
	cout << resultBody << endl;

	Json::Value obj;
	std::string error;
	Json::CharReaderBuilder crbuilder;
	std::unique_ptr<Json::CharReader> reader(crbuilder.newCharReader());
	reader->parse(resultBody.data(), resultBody.data() + resultBody.size(), &obj, &error);
	return obj["access_token"].asString();
}

#pragma optimize("",off)
void SendChat(string token) {
	CURL* curl = curl_easy_init();
	CURLcode res;

	string resultHeader;
	string resultBody;

	string url = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions_pro?access_token=" + token;
	const char* data = "{\"messages\" : [{\"role\" : \"user\",\"content\" : \"ÄãºÃ\"}]}";

	int size = MultiByteToWideChar(CP_ACP, 0, data, strlen(data) + 1, NULL, 0);
	wchar_t* wchar = new wchar_t[size];
	MultiByteToWideChar(CP_ACP, 0, data, strlen(data) + 1, wchar, size);

	int size2 = WideCharToMultiByte(CP_UTF8, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	char* utf8 = new char[size2 + 1];
	WideCharToMultiByte(CP_UTF8, 0, wchar, wcslen(wchar), utf8, size2, NULL, NULL);
	utf8[size2] = '\0';

	curl_slist* headers = nullptr;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, utf8);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &resultHeader);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	cout << resultHeader << endl;

	int size3 = MultiByteToWideChar(CP_UTF8, 0, resultBody.c_str(), strlen(resultBody.c_str()) + 1, NULL, 0);
	wchar_t* wchar3 = new wchar_t[size3];
	MultiByteToWideChar(CP_UTF8, 0, resultBody.c_str(), strlen(resultBody.c_str()) + 1, wchar3, size3);

	std::wcout.imbue(std::locale("chs"));
	wcout << wchar3 << endl;


}

int main() {

	string token = GetToken();
	SendChat(token);
	system("pause");
	return 0;
}

int Old() {
	CURL* curl = curl_easy_init();

	stringstream out;

	if (curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:7890");

		curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Authorization: Bearer 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");
		headers = curl_slist_append(headers, "OpenAI-Organization: org-8NFrG4csr9SaipTNY8iOCOFx");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		const char* json = "{																\
			\"model\": \"gpt-3.5-turbo\",													\
			\"messages\" : [{\"role\": \"user\", \"content\" : \"Say this is a test!\"}] ,	\
			\"temperature\" : 0.7}";
		curl_easy_setopt(curl, CURLOPT_POST, true);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);

		res = curl_easy_perform(curl);

		curl_slist_free_all(headers);

		string a = out.str();
		MessageBoxA(0, a.c_str(), 0, NULL);
	}
	return 0;
}
