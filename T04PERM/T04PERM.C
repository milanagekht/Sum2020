/* FILE NAME: T04PERM.C
 * PROGRAMMER: MG5
 * DATE: 02.06.2020 
 * PURPOSE: NONE
 */

#include<stdio.h>
#include <windows.h>
#define MAX 3

/* Permutation array */
BOOL IsParity; 
INT P[MAX];

VOID Store( VOID );
VOID Go( INT Pos );

/* Function that swaps element */
VOID Swap( INT *x, INT *y)
{
  INT tmp;

  tmp = *x;
  *x = *y;
  *y = tmp;
} /* End of 'Swap' function */


/* Function that makes new perm */
VOID Go( INT Pos )
{ 
  INT i, x;
  BOOL Save_Parity; 

/*  if (Pos == MAX)
    Store();
  else
  {   
    for(i = Pos; i < MAX; i++)
    {
      Swap(&P[Pos], &P[i]);
      IsParity = !IsParity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      IsParity = Save_Parity;
      IsParity = !IsParity;
    }
  }  */

  if (Pos == MAX)
  {
    Store();
    return;
  }
  else
  {
    /* First ocurance */
    Save_Parity = IsParity;
    Go(Pos + 1);
    for (i = Pos + 1; i < MAX; i++)
    { 
      /* Swap every position */
      Swap(&P[Pos], &P[i]);
      IsParity = !IsParity;
      Go(Pos + 1);
    } 
    /* Restore */
    x = P[Pos];
    for (i = Pos; i < MAX - 1; i++)
      P[i] = P[i + 1];
    P[MAX - 1] = x;
    IsParity = Save_Parity;
    
  } 
 } /* End of Go function */


/* Function that saves reult */
VOID Store( VOID )
{ 
  INT i;

  FILE *F = fopen("PERM.TXT", "a");
 
  if (F != NULL) 
  {
    for (i = 0; i < MAX - 1; i++)
      fprintf(F, "%d ", P[i]);
    fprintf(F, "%d  %s", P[MAX - 1], IsParity ? "Even" : "Odd");
    fprintf(F, "\n");
    fclose(F);
  }

  else 
    return;
} /* End of Save function */

/* The main function of the programm */
VOID main( VOID )
{
  INT i;

  for(i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
} /* End of main function */