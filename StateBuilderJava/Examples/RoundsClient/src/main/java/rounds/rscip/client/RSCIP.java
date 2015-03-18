package rounds.rscip.client;

public class RSCIP {
    public rscipContext context;
    private StateParameters sp;
    
    public RSCIP() {
    	this.sp = new StateParameters();
    	sp.setConferenceCreateTimeout(10L);
    	sp.setConferenceSetupTimeout(5L);
    	this.context = new rscipContext(sp);
    }
}