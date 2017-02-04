package filesAndApi;

import java.io.File;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;


public class ParseXML {
	
	public static String parseXml(String file){
		StringBuilder str = new StringBuilder();
		try {
			//Parse input file
			File inputFile = new File(file);
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(inputFile);

			doc.getDocumentElement().normalize();	
			String tag="";

			Node docNode = doc.getDocumentElement();
			str.append(docNode.getNodeName()+"\n\n");//Class

			NodeList nList=docNode.getChildNodes();
			tag=nList.item(1).getNodeName();//Student
			
			nList = doc.getElementsByTagName(tag);				
			for (int temp = 0; temp < nList.getLength(); temp++) {
				Node nNode = nList.item(temp);	
				str.append("current element: "+nNode.getNodeName()+"\n"); //Student
				if (nNode.getNodeType() == Node.ELEMENT_NODE){
					Element eElement = (Element) nNode;
					
					if (eElement.getAttributes().getLength()>0){
						Node elem=nNode.getAttributes().item(0);//rollnum
						str.append(elem.getNodeName()+": ");
						str.append(eElement.getAttribute(elem.getNodeName())+"\n");
					}
									
					NodeList elemList=eElement.getChildNodes();//first name, last name,...
					for (int i=1; i<elemList.getLength(); i+=2){
						String tagName=elemList.item(i).getNodeName();
						str.append(tagName+": ");//first name
						str.append(eElement.getElementsByTagName(tagName).item(0).getTextContent()+"\n");
					}
					str.append("\n");
				}
				else{
					break;
				}
			}
		} catch (Exception e) {	}
		
		return str.toString();
	}

	public static void main(String[] args){
		System.out.println(parseXml("input2.xml"));
	}
		
}
