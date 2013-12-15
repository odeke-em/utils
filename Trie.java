// Author: Emmanuel Odeke <odeke@ualberta.ca>
import java.util.ArrayList;

public class Trie<E> {
  public final int ASCII_INT_RANGE = '9' - '0';
  public final int ASCII_ALPHA_CHAR_RANGE = 'z' - 'a';

  private Trie[] base;
  private int index;
  private final int alphabetSize = ASCII_ALPHA_CHAR_RANGE + ASCII_INT_RANGE;

  public Trie(final int index) {
    if (index >= 0) {
      this.index = index;
      this.base = new Trie[this.alphabetSize];
    }
  }

  private void initBase() {
    for (int i=0; i < this.base.length; ++i) {
      this.base[i] = null;
    }
  } 

  private int indexResolve(char elem) {
    int elemIndex = -1;
    if (elem >= 'a' && elem <= 'z') {
      elemIndex = elem - 'a';
    } else if (elem >= '0' && elem <= '9') {
      elemIndex = ASCII_ALPHA_CHAR_RANGE + 1 + elem - '0';
    }

    return   this.base != null && elemIndex >= 0 
	  && elemIndex < this.base.length ? elemIndex : -1;
  }

  public void printTrie() {
    System.out.print(this.index + " { ");
    for (int i=0; i < this.base.length; ++i) {
      if (this.base[i] != null) this.base[i].printTrie();
    }
    System.out.println(" }");
  }

  public void insertSuffix(final String seq) {
    this.insertSequence(0, seq);
  }

  public boolean searchSuffix(final String query) {
    return this.search(0, query);
  }

  private boolean search(final int startIndex, final String query) {
    if (query != null && startIndex >= 0) {
      if (startIndex >= query.length()) { 
	return true;
      } else {
	char c = query.charAt(startIndex);
	int index = this.indexResolve(c);
	if (index < -1 || this.base[index] == null) return false;
	else return this.base[index].search(startIndex + 1, query);
      }
    } else {
      return false;
    }
  }

  public Trie getTrieAtIndex(int index) {
    if (index >= 0 && this.base != null && this.base.length > index) {
      return this.base[index];
    } else {
      return null;
    }
  }

  public ArrayList<Integer> openEntries() {
    ArrayList<Integer> openStorage = null;
    if (this.base != null) {
      openStorage = new ArrayList<Integer>();
      for (int i=0; i < this.base.length; ++i) {
	if (this.base[i] == null) openStorage.add(i);
      }
    }

    return openStorage;
  }

  private void insertSequence(final int startIndex, final String seq) {
    if (seq != null && startIndex >= 0  && startIndex < seq.length()) {
      char c = seq.charAt(startIndex);
      int index = this.indexResolve(c);
      if (index > -1) {
	if (this.base[index] == null) {
	  this.base[index] = new Trie(index);
	}
	
	this.base[index].insertSequence(startIndex + 1, seq);
      }
    }
  }
}
