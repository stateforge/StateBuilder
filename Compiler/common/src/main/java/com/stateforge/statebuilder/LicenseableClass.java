package com.stateforge.statebuilder;

import java.io.*;

/**
 * This class is shared freely under the Creative Commons Attribution 3.0 Unported License.
 * This means you can (a) use this class in commercial work,
 * and (b) modify this work, as long as (c) you attribute that this initial work
 * was created by Alvin Alexander of http://devdaily.com.
 * See http://creativecommons.org/licenses/by/3.0/ for more licensing information.
 *
 * This interface is intended to work with my LicenseController class.
 * A class that implements this interface can work with our LicenseController class.
 * Put another way, the LicenseController class only works with a class that 
 * implements this interface.
 */
public interface LicenseableClass
{
  public String getApplicationName();
  public InputStream getPublicKeystoreAsInputStream() throws FileNotFoundException;
  public String getAlias();
  public String getPublicKeystorePassword();
  public String getCipherParamPassword();
  public void handleVerifyLicenseFailedEvent(String error);
  public void handleVerifyLicenseRemoveFailedEvent(String error);
  public void handleLicenseTrialPeriodValid();
  public void handleLicenseValid();
  
  // ftp support
  public String getFtpAlias();
  public String getFtpKeyPwd();
}
