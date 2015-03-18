
class Microwave;

/** @class App is the entry point for the application.
 */
class App
{
  public:
    /** Default constructor
     */
    App();

    /** Default destructor
     */
    virtual ~App(){};
    
    /** App routine, 
     */
    int Main();

    void feedFsm(Microwave &microwave);

  private:
    void logInit();
};

