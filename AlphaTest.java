// Author: Emmanuel Odeke <odeke@ualberta.ca>

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;

public class AlphaTest {
  private static boolean DEBUG = false;
  public static void main(String[] args) {
    Trie t = new Trie(0);

    try {
      BufferedReader inBuf = new BufferedReader( new FileReader("Trie.java"));
      if (inBuf != null) {
	while (true) {
	  String lineIn = inBuf.readLine();
	  if (lineIn == null) break;

	  for (String word : lineIn.split(" ")) {
	    t.insertSuffix(word);
	    if (DEBUG) System.out.println("W: "+word);
	  }
	}
      }
    } catch (java.io.FileNotFoundException ex) {
      ;
    } catch (java.io.IOException ex) {
      ;
    }
    

    t.insertSuffix("odeke");
    t.insertSuffix("z");
    t.insertSuffix("10a");

    String[] elems = {"od", "eo", "flux"};

    for (String suffix : elems) {
      System.out.println(suffix + " " + t.searchSuffix(suffix));
    }

    t.printTrie();
    // 
    // ArrayList availableEntries = t.openEntries();
    // for (Object av : availableEntries) {
    //   System.out.println(av);
    // }
  
    Trie atIndex = t.getTrieAtIndex(5);
    if (atIndex != null) {
      atIndex.printTrie();
    }
  }
}
