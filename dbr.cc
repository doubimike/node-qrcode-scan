#include <node.h>
#include <string.h>
#include "If_DBR.h"
#include "BarcodeFormat.h"
#include "BarcodeStructs.h"
#include "ErrorCode.h"

using namespace v8;

void DecodeFile(const FunctionCallbackInfo<Value>& args) {

}

void Init(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "decodeFile", DecodeFile);
}

NODE_MODULE(dbr, Init)

Isolate* isolate = Isolate::GetCurrent();
HandleScope scope(isolate);
String::Utf8Value license(args[0]->ToString());
String::Utf8Value fileName(args[1]->ToString());
char *pFileName = *fileName;
char *pszLicense = *license;
__int64 llFormat = args[2]->IntegerValue();
Local<Function> cb = Local<Function>::Cast(args[3]);


int iMaxCount = 0x7FFFFFFF;
ReaderOptions ro = {0};
pBarcodeResultArray pResults = NULL;
ro.llBarcodeFormat = llFormat;
ro.iMaxBarcodesNumPerPage = iMaxCount;

DBR_InitLicense(pszLicense);
// Decode barcode image
int ret = DBR_DecodeFile(pFileName, &ro, &pResults);

const char * GetFormatStr(__int64 format)
{
    if (format == CODE_39)
        return "CODE_39";
    if (format == CODE_128)
        return "CODE_128";
    if (format == CODE_93)
        return "CODE_93";
    if (format == CODABAR)
        return "CODABAR";
    if (format == ITF)
        return "ITF";
    if (format == UPC_A)
        return "UPC_A";
    if (format == UPC_E)
        return "UPC_E";
    if (format == EAN_13)
        return "EAN_13";
    if (format == EAN_8)
        return "EAN_8";
    if (format == INDUSTRIAL_25)
        return "INDUSTRIAL_25";
    if (format == QR_CODE)
        return "QR_CODE";
    if (format == PDF417)
        return "PDF417";
    if (format == DATAMATRIX)
        return "DATAMATRIX";

    return "UNKNOWN";
}

Local<Array> barcodeResults = Array::New(isolate);

for (int i = 0; i < count; i++)
{
    tmp = ppBarcodes[i];

    Local<Object> result = Object::New(isolate);
    result->Set(String::NewFromUtf8(isolate, "format"), String::NewFromUtf8(isolate, GetFormatStr(tmp->llFormat)));
    result->Set(String::NewFromUtf8(isolate, "value"), String::NewFromUtf8(isolate, tmp->pBarcodeData));

    barcodeResults->Set(Number::New(isolate, i), result);
}

