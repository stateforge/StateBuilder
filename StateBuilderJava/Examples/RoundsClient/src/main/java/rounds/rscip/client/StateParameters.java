package rounds.rscip.client;

public class StateParameters {
	private ConferenceID conferenceId;
	private Long conferenceSetupTimeout;	// number of seconds to wait for a conference to begin.
	private Long conferenceCreateTimeout;	// number of seconds to wait for the conferenceCreate call to return.
	
	public ConferenceID getConferenceId() {
		return conferenceId;
	}

	public void setConferenceId(ConferenceID id) {
		this.conferenceId = id;
	}

	public Long getConferenceSetupTimeout() {
		return conferenceSetupTimeout;
	}

	public void setConferenceSetupTimeout(Long conferenceSetupTimeout) {
		this.conferenceSetupTimeout = conferenceSetupTimeout;
	}

	public Long getConferenceCreateTimeout() {
		return conferenceCreateTimeout;
	}

	public void setConferenceCreateTimeout(Long conferenceCreateTimeout) {
		this.conferenceCreateTimeout = conferenceCreateTimeout;
	}

	// Callbacks/Functions
	public void sendExitConferenceMessage(ConferenceID id) {
		System.out.printf("Sending exit for conf %s\n", id.getId());
	}
	
	public void sendConferenceCreateMessage(ParticipantID id) {
		System.out.printf("Sending create message with participant %s\n", id.getId());
	}
	
	public void notifyUserConferenceSetupTimeout(ConferenceID conf) {
		System.out.printf("Timeout while setting up conference %s\n", conf.getId());
	}
	
	public void notifyUserConferenceParticipant(final ConferenceID conf, final ParticipantID part, final ActionType act) {
		System.out.printf("Participant %s %s on conference %s\n", part.getId(), conf.getId(), act.toString());
	}
	
	public void notifyUserConferenceActive(final ConferenceID conf) {
		System.out.printf("Conference %s is active\n", conf.getId());
	}	
}
