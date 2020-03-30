//
//#include <memory>
//
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/dom/DOMElement.hpp>
//#include <xercesc/dom/DOMDocument.hpp>
//#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/sax2/SAX2XMLReader.hpp>
//#include <xercesc/sax2/XMLReaderFactory.hpp>
//#include <xercesc/sax/ErrorHandler.hpp>
//#include <xercesc/sax/SAXParseException.hpp>
//
//#include <xercesc/validators/common/Grammar.hpp>
//#include <xercesc/parsers/SAXParser.hpp>
//#include <xercesc/framework/MemBufInputSource.hpp>
//#include <xercesc/util/XMLString.hpp>
//
//#include <string>
//#include <iostream>
//
//#include "KCD_XSD.h"
//
//#include "../../include/dbcppp/Network.h"
//
//using namespace dbcppp;
//using namespace xercesc;
//
//class CErrorHandler : public xercesc::ErrorHandler 
//{
//public:
//   /** Warning message method */
//   void warning(const xercesc::SAXParseException& ex);
//   /** Error message method */
//   void error(const xercesc::SAXParseException& ex);
//   /** Fatal error message method */
//   void fatalError(const xercesc::SAXParseException& ex);
//   /** Errors resetter method */
//   void resetErrors();
//private:
//   /** Based message reporter method */
//   void reportParseException(const xercesc::SAXParseException& ex);
//};
//void CErrorHandler::reportParseException(const xercesc::SAXParseException& ex)
//{
//   char* message = xercesc::XMLString::transcode(ex.getMessage());
//   std::cout << message << " at line " << ex.getLineNumber() << " column " << ex.getColumnNumber() << std::endl;
//   xercesc::XMLString::release(&message);
//}
//void CErrorHandler::warning(const xercesc::SAXParseException& ex)
//{
//   reportParseException(ex);
//}
//void CErrorHandler::error(const xercesc::SAXParseException& ex)
//{
//   reportParseException(ex);
//}
//void CErrorHandler::fatalError(const xercesc::SAXParseException& ex)
//{
//   reportParseException(ex);
//}
//void CErrorHandler::resetErrors()
//{
//}
//class CXmlValidator
//{
//public:
//    /** Constructor method */
//    CXmlValidator();
//    /** Xml file setter method */
//    void setFilePath(const std::string &filePath);
//    /** Destructor method */
//    ~CXmlValidator();
//    /** Xml file with schema validation method */
//    bool validateSchema();
//private:
//    /** Xml file */
//    std::string mXMLFilePath;
//};
//CXmlValidator::CXmlValidator()
//    : mXMLFilePath("")
//{
//}
//CXmlValidator::~CXmlValidator()
//{
//}
//void CXmlValidator::setFilePath(const std::string &filePath)
//{
//    mXMLFilePath = filePath;
//}
//
//
//std::unique_ptr<Network> KCD2Network(DOMDocument& doc)
//{
//    std::set<std::unique_ptr<Node>> nodes;
//    auto root = doc.getDocumentElement();
//    DOMXPathResult* result = doc.evaluate(
//          XMLString::transcode("/NetworkDefinition/Node")
//        , root
//        , nullptr
//        , DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE
//        , nullptr);
//    for (std::size_t i = 0; i < result->getSnapshotLength(); i++)
//    {
//        result->snapshotItem(i);
//        auto attrs = result->getNodeValue()->getAttributes();
//        uint64_t id = 0;
//        std::string name = "";
//        std::string comment = "";
//        auto id_ = attrs->getNamedItem(u"id");
//        auto name_ = attrs->getNamedItem(u"name");
//        auto comment_ = attrs->getNamedItem(u"comment");
//        if (id_)
//        {
//            id = std::stoi(std::wstring((const wchar_t*)id_->getNodeValue()));
//        }
//        if (name_)
//        {
//            auto len = wcslen((wchar_t*)name_->getNodeValue());
//            name = std::string(len, '\0');
//            wcstombs(&name[0], (wchar_t*)name_->getNodeValue(), len + 1);
//        }
//        if (comment_)
//        {
//            auto len = wcslen((wchar_t*)comment_->getNodeValue());
//            comment = std::string(' ', len);
//            wcstombs(&comment[0], (wchar_t*)comment_->getNodeValue(), len);
//        }
//        auto n = Node::create(std::move(name), std::move(comment), {});
//        nodes.insert(std::move(n));
//    }
//    return nullptr;
//}
//std::unique_ptr<Network> Network::fromKCD(const std::string& filename)
//{
//    XMLPlatformUtils::Initialize();
//    auto parser = std::make_unique<XercesDOMParser>();
//    
//    parser->setValidationScheme(XercesDOMParser::Val_Auto);
//    parser->setDoSchema(true);
//    parser->setValidationConstraintFatal(true);
//    xercesc::ErrorHandler *handler = new CErrorHandler();
//    parser->setErrorHandler(handler);
//    parser->parse(filename.c_str());
//    if (parser->getErrorCount() != 0)
//    {
//        std::cout << "ERROR: XML file '" << filename << "' not confirm to the schema" << std::endl;
//    }
//    return KCD2Network(*parser->getDocument());
//}
