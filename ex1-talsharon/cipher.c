#include "cipher.h"
#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define MOD 26

/**
 * Encodes or decodes a capital letter by a given shift value
 * @param letter The given letter
 * @param k The shift value
 * @param is_cipher TRUE: if encode, FALSE: if decode
 * @return The encoded/decoded capital letter after shift
 */
char char_is_capital (char letter, int k, int is_cipher);

/**
 * * Encodes or decodes a lowercase letter by a given shift value
 * @param letter The given letter
 * @param k The shift value
 * @param is_cipher TRUE: if encode, FALSE: if decode
 * @return The encoded/decoded lowercase letter after shift
 */
char char_is_lowercase (char letter, int k, int is_cipher);

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.


// See full documentation in header file
void encode (char s[], int k)
{
  k = k % MOD;
  if (k != 0)
  {
    for (int i = 0; s[i] != '\0'; i++)
    {
      char *ptr = &s[i];
      char temp = *ptr;
      if (temp >= 'A' && temp <= 'Z')
      {
        s[i] = char_is_capital (temp, k, TRUE);
      }
      else if (s[i] >= 'a' && s[i] <= 'z')
      {
        s[i] = char_is_lowercase (s[i], k, TRUE);
      }
    }
  }
}

// See full documentation in header file
void decode (char s[], int k)
{
  k = k % MOD;
  if (k != 0)
  {
    for (int i = 0; s[i] != '\0'; i++)
    {
      char *ptr = &s[i];
      char temp = *ptr;
      if (temp >= 'A' && temp <= 'Z')
      {
        s[i] = char_is_capital (temp, k, FALSE);
      }
      else if (s[i] >= 'a' && s[i] <= 'z')
      {
        s[i] = char_is_lowercase (s[i], k, FALSE);
      }
    }
  }
}

char char_is_lowercase (char letter, int k, int is_cipher)
{
  char *ptr = &letter;
  char temp = *ptr;
  if (is_cipher == FALSE)
  {
    k = -k;
  }

  if (k > 0)
  {
    char remain = temp + k - 'z';
    if (remain > 0)
    {
      temp = 'a' + remain - 1;
    }
    else
    {
      temp = temp + k;
    }

  }
  else
  {
    char remain = 'a' - (temp + k);
    if (remain > 0)
    {
      temp = 'z' - remain + 1;
    }
    else
    {
      temp = temp + k;
    }
  }
  return temp;
}

char char_is_capital (char letter, int k, int is_cipher)
{
  char *ptr = &letter;
  char temp = *ptr;
  if (is_cipher == FALSE)
  {
    k = -k;
  }

  if (k > 0)
  {
    char remain = temp + k - 'Z';
    if (remain > 0)
    {
      temp = 'A' + remain - 1;
    }
    else
    {
      temp = temp + k;
    }

  }
  else
  {
    char remain = 'A' - (temp + k);
    if (remain > 0)
    {
      temp = 'Z' - remain + 1;
    }
    else
    {
      temp = temp + k;
    }
  }
  return temp;
}

