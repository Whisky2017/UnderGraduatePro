package test3;

import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.IOException;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Field.Store;
import org.apache.lucene.document.LongField;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.carrot2.core.Document;

//lucene 5.3
public class LuceneDemo3 {
	public static void main(String[] args) throws Exception {
		String indexDir1 = "D:\\data\\lucene\\odp\\index-all";
		//File indexDir2 =new File("D:\\data\\lucene\\odp\\index-all");
		String dataDir = "D:\\data";
	
		long start = System.currentTimeMillis();
		LuceneDemo3 indexer = new LuceneDemo3(indexDir1);
		
		int numIndexed;
		try {
			numIndexed = indexer.index(dataDir,new TextFilesFilter());
		} finally {
			indexer.close();
		}
		long end = System.currentTimeMillis();
		
		System.out.println("Indexing " + numIndexed + " files took " + (end-start) + " milliseconds.");
	}
	
	private File indexDir2 =new File("D:\\data\\lucene\\odp\\index-all");
	private IndexWriter writer;
	
	public LuceneDemo3(String indexDir) throws IOException {
		// TODO Auto-generated constructor stub
		//Directory directory = FSDirectory.open(new File(indexDir));
		Analyzer analyzer = new StandardAnalyzer();
		IndexWriterConfig config = new IndexWriterConfig(analyzer);
		config.setOpenMode(OpenMode.CREATE);
		writer = new IndexWriter(FSDirectory.open(indexDir2.toPath()),config);
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
			return path.getName().toLowerCase().endsWith(".txt");
		}	
	}
	
	private Document getDocument(Document f) throws Exception {
		// TODO Auto-generated method stub
		//Document document = new Document();
		final org.apache.lucene.document.Document doc = new org.apache.lucene.document.Document();	
		doc.add(new TextField("content", f.getSummary(), Store.NO));
        doc.add(new TextField("title", f.getTitle(), Store.YES));
        doc.add(new TextField("snippet", f.getSummary(), Store.YES));
        doc.add(new StringField("url", f.getContentUrl(), Store.YES));

		return doc;
	}
	
	private void indexFile(File f) throws Exception {
		// TODO Auto-generated method stub
		System.out.println("Indexing "+ f.getCanonicalPath());
		Document document = getDocument(f);
		writer.addDocument(document);
	}	

}
