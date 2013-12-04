package edu.iastate.cs352.project2.logic;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Queue;

import edu.iastate.cs352.project2.info.AddressInfo;
import edu.iastate.cs352.project2.info.ThreadInfo;
import edu.iastate.cs352.project2.logging.VirtualMemLogger;

/**
 * Class used to simulate a user process
 * @author Brandon
 *
 */
public class UserProcess {

	private final String FILE_NAME_PREPEND = "address_"; 
	
	private final int NUM_PAGES;
	
	private final int PAGE_SIZE;
	
	private static final int EMPTY_PAGE = -1;
	
	private static final int SLEEP_TIME = 1;
	
	// The info associated with this process
	private final ThreadInfo myInfo;
	
	// The list of addresses for this process to access
	private Queue<Integer> addresses;
	
	// THE PAGE TABLE! O.o
	private Integer[] pageTable;
	
	private final int OFFSET_MASK;
	
	private final int PAGE_NUM_MASK;
	
	/**
	 * Constructs a new User process with the given information
	 * @param processId
	 * @param pageSize
	 * @param numPages
	 */
	public UserProcess(int processId, int pageSize, int numPages) {
		this.PAGE_SIZE = pageSize;
		this.NUM_PAGES = numPages;
		
		this.myInfo = new ThreadInfo(processId);
		this.addresses = new LinkedList<Integer>();
		this.pageTable = new Integer[numPages];
		initPageTable();
		
		/*
		 *  Page sizes come in as a power of 2. The power is the number
		 *  of bits in the offset, so to mask out the offset we need a 
		 *  mask where the right-most bits are 1s.
		 *  
		 *   Similarly to mask out the pageNumber we need to top most 
		 *   bits as 1s
		 */
		OFFSET_MASK = (1 << (int) logBase2(PAGE_SIZE)) - 1;
		PAGE_NUM_MASK = ~OFFSET_MASK;
	}
	
	// fills in the page table as empty
	private void initPageTable() {
		for (int i = 0; i < NUM_PAGES; ++i) {
			pageTable[i] = EMPTY_PAGE;
		}
	}
	
	public void readAddresses() {
		/*
		 * Read all of the addresses into memory to avoid dealing with the overhead
		 * for reading a file
		 */
		try (BufferedReader br = new BufferedReader(new FileReader(FILE_NAME_PREPEND + myInfo.getProcessId()))) {
			String read = "";
			while ((read = br.readLine()) != null) {
				addresses.offer(Integer.parseInt(read));
			}
		} catch (IOException ioe) {
			System.out.println("Problem attempting to read file!");
			ioe.printStackTrace();
		} catch (NumberFormatException nfe) {
			System.out.println("An address wasn't an integer!");
			nfe.printStackTrace();
		}
	}
	
	/**
	 * Starts the execution of this process
	 */
	public void run() {
		// We'll remove addresses from the queue once we know they're hits
		while (!addresses.isEmpty()) {
			int curAddr = addresses.peek();
			int pageNum = calcPageNumber(curAddr);
			int offset = calcOffset(curAddr);
			int frame = pageTable[pageNum];
			AddressInfo curInfo = new AddressInfo(curAddr, pageNum, offset, frame, this.myInfo);
			
			// TODO handle other threads messin with my shit
			if (!pageFault(pageNum)) {
				VirtualMemLogger.logSuccessfulAccess(curInfo);
				// Actually remove the address since we only peeked before
				addresses.poll();
				continue;
			}
			
			// It was a failure so we need to load the page into memory
			frame = MemoryManagmentUnit.loadPage(curAddr, this.myInfo);
			waitForLoad();
			
			// update my page table with the new value
			pageTable[pageNum] = frame;
			
			/*
			 * End of loop, we'll try the address again, next time it 
			 * should be a hit since we just pulled the frame into
			 * memory
			 */
		}
	}
	
	private boolean pageFault(int pageNum) {
		return pageTable[pageNum] != EMPTY_PAGE;
	}
	
	private void waitForLoad() {
		try {
			Thread.sleep(SLEEP_TIME);
		} catch (InterruptedException e) {
			/*
			 * I'm guessing this doesn't matter since the sleep is just
			 * pretend anyway 
			 */
		}
	}
	
	private int calcPageNumber(int address) {
		// The bitwise operation >>> shifts in 0's on the right
		return (address & PAGE_NUM_MASK) >>> logBase2(PAGE_SIZE);
	}
	
	private int calcOffset(int address) {
		return address & OFFSET_MASK;
	}
	
	private int logBase2(int x) {
		return (int)(Math.log(x)/Math.log(2));
	}
}
