package edu.iastate.cs352.project2.info;

/**
 * Boxes up information about a specific address's page number,
 * offset, physical memory frame number, and thread info
 * @author Brandon
 *
 */
public class AddressInfo {

	private int address;
	private int pageNumber;
	private int offset;
	private int frameNumber;
	
	private ThreadInfo threadInfo;
	
	public AddressInfo(int address, int pageNumber, int offset, int frameNumber, ThreadInfo threadInfo) {
		this.address = address;
		this.pageNumber = pageNumber;
		this.offset = offset;
		this.frameNumber = frameNumber;
		this.threadInfo = threadInfo;
	}

	public int getAddress() {
		return address;
	}
	
	public int getPageNumber() {
		return pageNumber;
	}

	public int getOffset() {
		return offset;
	}

	public int getFrameNumber() {
		return frameNumber;
	}

	public ThreadInfo getThreadInfo() {
		return threadInfo;
	}
}
