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

	private static final String FILE_NAME_PREPEND = "address_";
	
	// The info associated with this process
	private ThreadInfo myInfo;
	
	// The list of addresses for this process to access
	private Queue<Integer> addresses;
	
	public UserProcess(int processId) {
		myInfo = new ThreadInfo(processId);
		addresses = new LinkedList<Integer>();
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
}
