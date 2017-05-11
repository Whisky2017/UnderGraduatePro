package test2;

import org.apache.lucene.index.IndexFileNames;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.document.Field.Store;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.Directory;
//import org.carrot2.core.Document;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.io.FileReader;
import java.io.BufferedReader; 
import java.io.InputStreamReader;  




//lucene 5.3
public class LuceneDemo2 {
	public static void main(String[] args) throws Exception {
		String indexDir = "D:\\data\\lucene\\odp\\index-all";
		//String encryptDir = "D:\\data\\lucene\\odp\\encrypt";
		String dataDir = "D:\\data\\PDF\\Tiger";
	
		long start = System.currentTimeMillis();
		LuceneDemo2 indexer = new LuceneDemo2(indexDir);
		
		int numIndexed;
		try {
			numIndexed = indexer.index(dataDir,new TextFilesFilter());
		} finally {
			indexer.close();
		}
		long end = System.currentTimeMillis();
		
		System.out.println("Indexing " + numIndexed + " files took " + (end-start) + " milliseconds.");
		
	}
	
	private IndexWriter writer;
	
	public LuceneDemo2(String indexDir) throws IOException {
		// TODO Auto-generated constructor stub
		Directory directory = FSDirectory.open(new File(indexDir).toPath());
		Analyzer analyzer = new StandardAnalyzer();
		IndexWriterConfig config = new IndexWriterConfig(analyzer);
		config.setOpenMode(OpenMode.CREATE);
		writer = new IndexWriter(directory,config);
	}
	
	public void close() throws IOException {
		writer.close();
	}
	
	public int index (String dataDir,FileFilter filter) throws Exception {
		File[] files = new File(dataDir).listFiles();
		
		//if(files == null) return writer.numDocs();
		for(File f: files) {
			if(!f.isDirectory()&&
				!f.isHidden()&&
				f.exists()&&
				f.canRead()&&
				(filter == null || filter.accept(f))) {
				indexFile(f);
			}
		}
		
		/*
		if(files == null) return writer.numDocs();
		for(int i=0;i<files.length&&files!=null;i++) {
			if(!files[i].isDirectory()&&
				!files[i].isHidden()&&
				files[i].exists()&&
				files[i].canRead()&&
				(filter == null || filter.accept(files[i]))) {
				indexFile(files[i]);
			}
		}
		*/
		return writer.numDocs();
	}
	
	private static class TextFilesFilter implements FileFilter {
		public boolean accept(File path) {
			return path.getName().toLowerCase().endsWith(".pdf");
		}	
	}
	/**
	private Document getDocument(File f) throws Exception {
		// TODO Auto-generated method stub
		String xpdfPath = "D:\\xpdf\\xpdfbin-win-3.04\\bin64\\pdfintext.exe ";  
		String PDFname = f.getName()+".txt";
		String cmd = xpdfPath + f.getAbsolutePath()+" "+PDFname; 
		System.out.println(cmd);
        Process process = Runtime.getRuntime().exec(cmd);  
        
		//org.apache.lucene.document.Document d  =  new org.apache.lucene.document.Document();
		//Document d = LucenePDFDocument.getDocument(f);
        //System.out.println(br.toString());
		//Document document = new Document();
		final org.apache.lucene.document.Document document = new org.apache.lucene.document.Document();
		document.add(new StringField("url",  f.getAbsolutePath(), Store.YES));
		//document.add(new LongField("modified", f.lastModified(), Store.NO)); 
		//document.add(new TextField("content", d.toString(),Store.YES));
		document.add(new TextField("title", f.getName(), Store.YES));
		/* 
		doc.add(new TextField("content", f.getSummary(), Store.NO));
        doc.add(new TextField("title", f.getTitle(), Store.YES));
        doc.add(new TextField("snippet", f.getSummary(), Store.YES));
        doc.add(new StringField("url", f.getContentUrl(), Store.YES));
		 *

		return document;
	}
	*/
	
	public static String stripNonValidXMLCharacters(String in) {
	    StringBuffer out = new StringBuffer(); // Used to hold the output.
	    char current; // Used to reference the current character.

	    if (in == null || ("".equals(in)))
	        return ""; // vacancy test.
	    for (int i = 0; i < in.length(); i++) {
	        current = in.charAt(i); // NOTE: No IndexOutOfBoundsException caught
	                                // here; it should not happen.
	        if ((current == 0x9) || (current == 0xA) || (current == 0xD)
	                || ((current >= 0x20) && (current <= 0xD7FF))
	                || ((current >= 0xE000) && (current <= 0xFFFD))
	                || ((current >= 0x10000) && (current <= 0x10FFFF)))
	            out.append(current);
	    }
	    return out.toString();
	}
	
	private void indexFile(File f) throws Exception {
		// TODO Auto-generated method stub
		System.out.println("Indexing "+ f.getCanonicalPath());
		//Document d = LucenePDFDocument.getDocument(f);
		String executeStr = "D:\\xpdf\\xpdfbin-win-3.04\\bin64\\pdftotext.exe";
		String[] cmd = new String[]{executeStr,"-enc","UTF-8","-q",f.getAbsolutePath(),"-"};  
		String str = null ; 
		Process p = null ;     
	    BufferedReader br = null ;  
	    StringBuffer sb = new StringBuffer() ;
        try {  
            p = Runtime.getRuntime().exec(cmd) ;               
            br = new BufferedReader(new InputStreamReader(p.getInputStream(),"UTF-8")) ;    
            while((str = br.readLine() ) != null ){  
                sb.append(str).append("\n") ;  
            }               
        } catch (IOException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } finally{  
            if (br != null){  
                try {  
                    br.close() ;  
                } catch (IOException e) {  
                    // TODO Auto-generated catch block  
                    e.printStackTrace();  
                }  
            }  
        }  
        String content = sb.toString();
        content = stripNonValidXMLCharacters(content);
        Document document = new Document();
        document.add(new StringField("url",  f.getAbsolutePath(), Store.YES));
		document.add(new TextField("content", content,Store.YES));
		document.add(new TextField("title", f.getName(), Store.YES));
		writer.addDocument(document);
	}
}
