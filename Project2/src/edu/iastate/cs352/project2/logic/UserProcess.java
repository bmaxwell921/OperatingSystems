package edu.iastate.cs352.project2.logic;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Queue;

import edu.iastate.cs352.project2.info.ThreadInfo;

/**
 * Class used to simulate a user process
 * @author Brandon
 *
 */
public class UserProcess {

	private final String FILE_NAME_PREPEND = "address_";
	
	private final int NUM_PAGES;
	
	private final int PAGE_SIZE;
	
	// The info associated with this process
	private final ThreadInfo myInfo;
	
	// The list of addresses for this process to access
	private Queue<Integer> addresses;
	
	// THE PAGE TABLE! O.o
	private Integer[] pageTable;
	
	private final int offsetMask;
	
	private final int pageNumberMask;
	
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
		this.pageTable = new Integer[numPages];
		this.addresses = new LinkedList<Integer>();
		
		// TODO see blah project
//		offsetMask = (1 << ((int) Math.log(PAGE_SIZE)) + 1) - 1;
//		pageNumberMask = ~offsetMask;
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
		// TODO
	}
	
	private int calcPageNumber(int address) {
		return -1;
	}
	
	private int calcOffset(int address) {
		return -1;
	}
}
