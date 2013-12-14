// Author: Emmanuel Odeke <odeke@ualberta.ca>

#ifndef _WORD_TRANSITION_H
#define _WORD_TRANSITION_H
  typedef unsigned int uint32;
  typedef struct {
    uint32 moves, 
	  reuses,
	  inplace,
	  deletions;
  } EditStat;

  void initEditStat(EditStat *est);
  inline EditStat *allocEditStat(void); 
  EditStat *allocAndInitEditStat(void);
  void printStat(const EditStat *);
  EditStat *getEditStats(const char *subject, const char *base);
#endif
