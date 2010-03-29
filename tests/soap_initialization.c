#include <ws/soapH.h>
#include <ws/ns3.nsmap>


int main()
{
	struct soap *soap; // gSOAP runtime environment

	soap = soap_new();
	if (!soap) {
		return 1;
	}

	soap_destroy(soap);
	soap_end(soap);
	soap_done(soap);
}
