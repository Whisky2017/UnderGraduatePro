package test;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.io.FileReader;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.queryparser.classic.MultiFieldQueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.highlight.Highlighter;
import org.apache.lucene.search.highlight.InvalidTokenOffsetsException;
import org.apache.lucene.search.highlight.QueryScorer;
import org.apache.lucene.search.highlight.SimpleFragmenter;
import org.apache.lucene.search.highlight.SimpleHTMLFormatter;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.RAMDirectory;
import org.apache.lucene.util.Constants;
import org.apache.lucene.util.Version;

public class LuceneDemo {
	public static void main(String[] args) throws IOException,ParseException,InvalidTokenOffsetsException{
		String indexDir = "D:\\data\\lucene\\odp\\index-all";
		String dataDir = "D:\\data";
		
		Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_4_9);
		
		//Directory directory = new RAMDirectory();
		Directory directory = FSDirectory.open(new File(indexDir));
		IndexWriterConfig config = new IndexWriterConfig(Version.LUCENE_4_9,analyzer);
		config.setOpenMode(OpenMode.CREATE);
		IndexWriter iwriter = new IndexWriter(directory, config);
		Document doc = new Document();
        
        DirectoryReader ireader = DirectoryReader.open(directory);
        IndexSearcher isearcher = new IndexSearcher(ireader);
        String[] multiFields = {"fieldname", "content"};
        MultiFieldQueryParser parser = new MultiFieldQueryParser(Version.LUCENE_4_9, multiFields, analyzer);
        Query query = parser.parse("卧槽，中国");
        TopDocs docs = isearcher.search(query, null, 10);
        ScoreDoc[] hits = docs.scoreDocs;
        
        SimpleHTMLFormatter simpleHTMLFormatter = new SimpleHTMLFormatter("<span style='color:green'>", "</span>");
        Highlighter highlighter = new Highlighter(simpleHTMLFormatter, new QueryScorer(query));
        highlighter.setTextFragmenter(new SimpleFragmenter(100));  
        
        System.out.println("Searched " + hits.length + " documents. ");
        for(int i=0;i<hits.length;i++){
        	Document hitdoc = isearcher.doc(hits[i].doc);
        	String[] scoreExplain = null;
        	scoreExplain = isearcher.explain(query, hits[i].doc).toString().split(" ", 2);
        	String scores = scoreExplain[0];
        	System.out.println("score " + scores);
        	String value = hitdoc.get("fieldname");
        	TokenStream tokenStream = analyzer.tokenStream(value, new StringReader(value));
        	String str1 = highlighter.getBestFragment(tokenStream, value);
        	
        	System.out.println(str1);	
         }
        ireader.close();
        directory.close();   
	}	
}
