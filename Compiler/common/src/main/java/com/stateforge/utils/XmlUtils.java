package com.stateforge.utils;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.w3c.dom.Document;
import org.xml.sax.ErrorHandler;
import org.xml.sax.SAXException;

public class XmlUtils {
	static public Document documentCreate(InputStream xmlInputStream, InputStream schemaInputStream, ErrorHandler errorHandler)
			throws ParserConfigurationException, SAXException, IOException {
		Document doc = null;
		String XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID = "http://apache.org/xml/features/xinclude/fixup-base-uris";

		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		dbf.setNamespaceAware(true);
		dbf.setXIncludeAware(true);
		
		// create a SchemaFactory capable of understanding Xsd schemas
		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);

		// load a schema, represented by a Schema instance
		Source schemaSource = new StreamSource(schemaInputStream);
		Schema schema = factory.newSchema(schemaSource);

		dbf.setSchema(schema);	
		
		//Do not use DTD validation but the xsd one
		dbf.setValidating(false); 
		
		/*
		 * since the config file uses xinclude and validation is on, set
		 * this feature to false, otherwise validation fails
		 */
		dbf.setFeature(XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID, false);

		DocumentBuilder db = dbf.newDocumentBuilder();
		
		db.setErrorHandler(errorHandler);
		
		int inputSize = xmlInputStream.available();
		if(inputSize < 0){
			return null;
		}
		
		byte[] buffer = new byte[inputSize]; 
		xmlInputStream.read(buffer, 0, inputSize);
		ByteArrayInputStream xmlValidationInputStream = new ByteArrayInputStream(buffer);
		ByteArrayInputStream xmlSaxInputStream = new ByteArrayInputStream(buffer);
		
		//xmlValidationInputStream.
		// Step 3: Parse the input file to only for xsd validation.
		db.parse(xmlValidationInputStream);

		//Now create a Dom with the line and column number:
    	SAXParser saxParser = SAXParserFactory.newInstance().newSAXParser();
    
    	doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
    	
		LocationRecordingHandler handler = new LocationRecordingHandler(doc); 
		saxParser.parse(xmlSaxInputStream, handler);
		
		return doc;
	}
}
