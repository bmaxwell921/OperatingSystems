package edu.iastate.cs352.project2.logging;

import edu.iastate.cs352.project2.info.AddressInfo;
import edu.iastate.cs352.project2.info.ThreadInfo;

/**
 * Convenience class used to log messages
 * 
 * @author Brandon
 * 
 */
public class VirtualMemLogger {

	// Message parts
	private static final String PROCESS_MSG = "Process %d";
	private static final String ADDRESS_MSG = "address %d";
	private static final String PAGE_NUM_MSG = "page number = %d";
	private static final String PAGE_OFFSET_MSG = "page offset = %d";
	private static final String FRAME_NUM_MSG = "frame number = %d";
	
	// Actual outputs
	private static final String SUCCESS_ACCESS_MSG = "[" + PROCESS_MSG + "] " + "accesses " + ADDRESS_MSG + " " + "(" + PAGE_NUM_MSG + ", "
			+ PAGE_OFFSET_MSG + ") "+ "in main memory " + "(" + FRAME_NUM_MSG + ").";
	
	private static final String PAGE_FAULT_MSG = "[" + PROCESS_MSG + "] " + "accesses " + ADDRESS_MSG + " " + "(" + PAGE_NUM_MSG + ", "
			+ PAGE_OFFSET_MSG + ") " + "not in main memory.";
	
	private static final String FOUND_FRAME_MSG = "[" + PROCESS_MSG + "] " + "finds a free frame in main memory " 
			+ "(" + FRAME_NUM_MSG + ").";
	private static final String REPLACE_FRAME_MSG = "[" + PROCESS_MSG + "] " + "replaces a frame " + "(" + FRAME_NUM_MSG  + ")"
			+ " from the main memory.";
	private static final String ISSUE_SWAP_MSG = "[" + PROCESS_MSG + "] " + "issues an I/O operation to swap in demanded page "
			+ "(" + PAGE_NUM_MSG + ").";
	private static final String COMPLETE_SWAP_MSG = "[" + PROCESS_MSG + "] " + "demanded page " + "(" + PAGE_NUM_MSG
			+ ") has been swapped in main memory " + "(" + FRAME_NUM_MSG + ").";
	private static final String PROCESS_ENDS_MSG = "[" + PROCESS_MSG + "] " + "ends.";
	
	/**
	 * Used to log when an address is found already in main memory
	 * @param info
	 */
	public static synchronized void logSuccessfulAccess(AddressInfo info) {
		formatAndLog(SUCCESS_ACCESS_MSG, info.getThreadInfo().getProcessId(), info.getAddress(),
				info.getPageNumber(), info.getOffset(), info.getFrameNumber());
	}
	
	/**
	 * Used to log the page fault message
	 * @param info
	 */
	public static synchronized void logPageFault(AddressInfo info) {
		formatAndLog(PAGE_FAULT_MSG, info.getThreadInfo().getProcessId(), info.getAddress(), 
				info.getPageNumber(), info.getOffset(), info.getFrameNumber());
	}
	
	/**
	 * Used to log the free frame found message
	 * @param info
	 * @param frameNum
	 */
	public static synchronized void logFoundFrame(ThreadInfo info, int frameNum) {
		formatAndLog(FOUND_FRAME_MSG, info.getProcessId(), frameNum);
	}
	
	/**
	 * Used to log the message when a frame is replaced in main memory
	 * @param info
	 * @param frameNum
	 */
	public static synchronized void logReplaceFrame(ThreadInfo info, int frameNum) {
		formatAndLog(REPLACE_FRAME_MSG, info.getProcessId(), frameNum);
	}
	
	/**
	 * Used to log the message when a thread issues and I/O request to swap in a new frame
	 * @param info
	 */
	public static synchronized void logIssueSwap(AddressInfo info) {
		formatAndLog(ISSUE_SWAP_MSG, info.getThreadInfo().getProcessId(), info.getPageNumber());
	}
	
	/**
	 * Used to log the message when a new frame has been swapped into main memory
	 * @param info
	 * @param frameNumber
	 */
	public static synchronized void logCompletedSwap(AddressInfo info, int frameNumber) {
		formatAndLog(COMPLETE_SWAP_MSG, info.getThreadInfo().getProcessId(), info.getPageNumber(), 
				frameNumber);
	}
	
	/**
	 * Used to log the message when a process is complete
	 * @param info
	 */
	public static synchronized void logProcessEnds(ThreadInfo info) {
		formatAndLog(PROCESS_ENDS_MSG, info.getProcessId());
	}
	
	// Common method used to cut down on duplicated code
	private static synchronized void formatAndLog(String message, Object... args) {
		System.out.println(String.format(message, args));
	}
}
