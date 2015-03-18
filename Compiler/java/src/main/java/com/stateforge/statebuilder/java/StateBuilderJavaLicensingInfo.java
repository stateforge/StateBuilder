
package com.stateforge.statebuilder.java;

import com.stateforge.statebuilder.LicenseableClass;
import java.io.FileNotFoundException;
import java.io.InputStream;

/**
 *
 * @author frederic
 */
public class StateBuilderJavaLicensingInfo implements LicenseableClass {

    // license stuff - uses the ObfuscatedString class from the True License
    // library
    // private static String keystoreFilename = new ObfuscatedString(new long[]
    // {0x2531E65E362C302AL, 0xC6491960E7CAB920L,
    // 0xC3BA520AAFF63939L}).toString(); /* => "dc.properties" */
    private static String keystoreFilename = "StateBuilderJavaPublicKeyStore";
    // private static String alias = new ObfuscatedString(new long[]
    // {0xDE2992A6F9C3C3A1L, 0xA08CA77A6F05791AL,
    // 0x59E2378BA6F1BA96L}).toString(); /* => "dcpublickey" */
    private static String alias = "StateBuilderJava";
    // private static String publicCertStorePassword = new ObfuscatedString(new
    // long[] {0x43426A6C456239FAL, 0x98099DA1F000C879L, 0x4B8DCA266320210EL,
    // 0xC13BE948052C1673L}).toString(); /* => "Z3N H4ND M00N 6590" */
    private static String publicCertStorePassword = "Janu2010";
    // private static String cipherParamPassword = new ObfuscatedString(new
    // long[] {0xCF899EE49F709ACAL, 0x954921EB531BF4BFL,
    // 0x3D73B0811299826DL}).toString(); /* => "d0sxxb33rm3si" */
    // private static String cipherParamPassword = "d0s4ny1kn0wth1s";
    private static String cipherParamPassword = "d0s4ny1kn0wth1s";
    // ftp license stuff - uses the ObfuscatedString class from the True License
    // library
    // private static String ftpAlias = new ObfuscatedString(new long[]
    // {0xA4B0FBA523B3F63AL, 0x65D5A72B8657E00BL,
    // 0x9E71F6D2395FF6FL}).toString(); /* => "dcftpkeyv1.x" */
    private static String ftpAlias = "StateBuilderJavaFtp";
    // private static String ftpKeyPwd = new ObfuscatedString(new long[]
    // {0xA2B75E0F17691BD4L, 0xEB9DB7C881078526L, 0x5ED8D7B254BC11BEL,
    // 0x4422FAD5CD5DC734L}).toString(); /* => "J4V D3RB 88743 1411" */
    private static String ftpKeyPwd = publicCertStorePassword;

    public InputStream getPublicKeystoreAsInputStream()
            throws FileNotFoundException {
        final String resourceName = keystoreFilename;
        final InputStream in = getClass().getClassLoader().getResourceAsStream(
                resourceName);
        if (in == null) {
            throw new FileNotFoundException(resourceName);
        }
        return in;
    }

    public String getApplicationName() {
        return alias;
    }

    public String getAlias() {
        return alias;
    }

    public String getPublicKeystorePassword() {
        return publicCertStorePassword;
    }

    public String getCipherParamPassword() {
        return cipherParamPassword;
    }

    public Class<?> getClassToLicense() {
        return this.getClass();
    }

    public String getFtpKeyPwd() {
        return ftpKeyPwd;
    }

    public String getFtpAlias() {
        return ftpAlias;
    }

    @Override
    public void handleLicenseTrialPeriodValid(){
        //Logger.getLogger("StateBuilderJava").log(Level.INFO,"");
    }
    
    @Override
    public void handleLicenseValid(){
        //Logger.getLogger("StateBuilderJava").log(Level.INFO, "");
    }

	@Override
	public void handleVerifyLicenseFailedEvent(String error) {
		System.err.println(alias + ": Cannot install licence");
		if(error != null){
			System.err.println("Error " + error);
		}
	}

	@Override
	public void handleVerifyLicenseRemoveFailedEvent(String error) {
		System.err.println(alias + ": Cannot remove licence");
		if(error != null){
			System.err.println("Error " + error);
		}		
	}

}
