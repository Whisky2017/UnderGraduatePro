package test2;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.Key;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.CipherOutputStream;
import javax.crypto.KeyGenerator;


//encrypt with DES
public class Encrypt {
	Key key;
	public Encrypt(String str){
		getKey(str);
	}
	public void getKey(String strKey){
		try{
			KeyGenerator _gGenerator = KeyGenerator.getInstance("DES");
			_gGenerator.init(new SecureRandom(strKey.getBytes()));
			this.key = _gGenerator.generateKey();
			_gGenerator = null;
		}catch (Exception e ){
			throw new RuntimeException("Error initializing SqlMap class. Cause: "+e);
		}
	}
	
	public void jiami(String file,String destFile) throws Exception {
		Cipher cipher = Cipher.getInstance("DES");
		cipher.init(Cipher.ENCRYPT_MODE, this.key);
		InputStream iStream =  new FileInputStream(file);
		OutputStream oStream = new FileOutputStream(destFile);
		CipherInputStream cStream = new CipherInputStream(iStream, cipher);
		byte[] buffer = new byte[1024];
		int r;
		while ((r = cStream.read(buffer))>0){
			oStream.write(buffer, 0, r);
		}
		cStream.close();
		iStream.close();
		oStream.close();
	}
	
	public void jiemi(String file,String destFile) throws Exception {
		Cipher cipher = Cipher.getInstance("DES");
		cipher.init(Cipher.DECRYPT_MODE, this.key);
		InputStream iStream = new FileInputStream(file);
		OutputStream oStream = new FileOutputStream(destFile);
		CipherOutputStream cStream = new CipherOutputStream(oStream, cipher);
		byte[] buffer = new byte[1024];
		int r;
		while((r = iStream.read(buffer))>=0){
			System.out.println();
			cStream.write(buffer, 0, r);
		}
		cStream.close();
		iStream.close();
		oStream.close();
	}
	
	public static void main(String[] args) throws Exception {	
		String dataDir = "D:\\data\\lucene\\odp\\index-all";
		String encryptDir = "D:\\data\\lucene\\odp\\encrypt\\file";
		int num=0;
		
		File[] files = new File(dataDir).listFiles();
		for(File f: files) {
			if(!f.isDirectory()&&
				!f.isHidden()&&
				f.exists()&&
				f.canRead()) {
				Encrypt td = new Encrypt("aaa");
				String file = f.getPath();
				//System.out.println(file);
				num++;
				String destFile = encryptDir + num +".txt" ;
				td.jiami(file, destFile);
				System.out.println("Success encrypt "+ file + " to " + destFile);	
			}
		}
		System.out.println("Success encrypt "+num+" files!");
	}
}