
#include "AcsAppBase.h"


class AcsParamDownload;

using namespace std;

class AcsDownloadApp : public AcsAppBase {
  
  Q_OBJECT 
  
  public:
    
    AcsDownloadApp(const QString &host);
    
    void addRequest(AcsParamDownload &paramDownload);
};

