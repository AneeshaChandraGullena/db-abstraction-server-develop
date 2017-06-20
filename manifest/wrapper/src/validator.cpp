/* © Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM

Simple program to check the syntax of the Service Manifest JSON file.
This program is linked with the Service Manifest runtime library which contains the parser code
It is also linked with the binary blob containing the JSON data so it can be accessed as an array

Rules based on this page
https://github.ibm.com/CloudTools/Team/issues/38
with input from
https://github.ibm.com/ibmcloud/builders-guide/blob/master/specifications/crn/CRN.md

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *function_names[] = {"$env", "$hostname", NULL};
const char *keyword_choices_tenancy[] = {"single", "shared", NULL};
const char *keyword_choices_ctype[] = {"public", "dedicated", "local", NULL};

typedef enum {VERSION, STRICT, ALPHANUM, BASIC, EMAIL, URL, KEYWORD, SCOPE} manifest_value_type;

typedef struct element_data
{
  const char *name;
  bool is_required;
  bool is_crn;
  bool allow_func;
  manifest_value_type value_type;
  const char **keyword_choices;
} element_data;

//  name                required    crn     func    type      keyword_choices
element_data manifest_elements[] = {
  {"manifest_version",      true,   false,  false,  VERSION,  NULL},
  {"service_name",          true,   false,  false,  STRICT,   NULL},
  {"component_name",        true,   false,  false,  STRICT,   NULL},
  {"cname",                 false,  true,   true,   BASIC,    NULL},
  {"ctype",                 false,  true,   true,   KEYWORD,  keyword_choices_ctype},
  {"component_instance",    false,  true,   true,   BASIC,    NULL},
  {"scope",                 false,  true,   true,   SCOPE,    NULL},
  {"region",                false,  true,   true,   BASIC,    NULL},
  {"resource_type",         false,  true,   false,  STRICT,   NULL},
  {"resource",              false,  true,   true,   BASIC,    NULL},
  {"tenancy",               false,  false,  false,  KEYWORD,  keyword_choices_tenancy},
  {"pager_duty",            false,  false,  false,  URL,      NULL},
  {"team_email",            false,  false,  false,  EMAIL,    NULL},
  {"bailey_url",            false,  false,  false,  URL,      NULL},
  {"bailey_project",        false,  false,  false,  BASIC,    NULL},
  {"repo_url",              false,  false,  false,  URL,      NULL},
  {"softlayer_account",     false,  false,  true,   BASIC,    NULL},
  {"bluemix_account",       false,  false,  true,   BASIC,    NULL},
  {"security_status_url",   false,  false,  false,  URL,      NULL},
  {"static_sec_analyze_url", false, false,  false,  URL,      NULL},
  {"customer_ticket_repo_url",false,false,  false,  URL,      NULL},
  {"runbook_repo_url",      false,  false,  false,  URL,      NULL},
  {"continuity_plan_url",   false,  false,  false,  URL,      NULL},
  {NULL,                    false,  false,  false,  BASIC,    NULL}
};

#define VALID_URL_CHARACTERS "_.~!*'();:@&=+$,/?#[]%"
#define VALID_EMAIL_CHARACTERS "!#$%&'*+/=?^_`{|}~."


// These 2 symbols point to the data object containing the JSON data
extern const char manifest_data[]     asm("_binary____res_servicemanifest_json_start");
extern const char manifest_data_end[] asm("_binary____res_servicemanifest_json_end");
//extern const char manifest_data[]       asm("_binary_______servicemanifest_json_start");
//extern const char manifest_data_end[]   asm("_binary_______servicemanifest_json_end");

//   Simple function to search a JSON string for 1st occurance
//   of a key and return the value.  Ignores JSON structure.
static char *find_json_value(char *json_str, const char *key)
{
  // add quotes to the search key
  int fullkey_len = strlen(key) + 2;
  char *fullkey = new char[fullkey_len+1];
  sprintf(fullkey, "\"%s\"", key);

  // search for first occurance of the key (ignores JSON structure)
  char *ptr = strstr(json_str, fullkey);
  delete[] fullkey;
  if (ptr == NULL) return NULL;

  char *start = ptr + fullkey_len;
  while (*start == ' ' || *start == '\t' || *start == ':')
  {
    start++;
  }
  char *end = NULL;
  if (*start == '"') end = strchr(++start, '"');  // if value starts with a quote, skip it and look for closing quote
  if (end == NULL) end = strchr(start, ',');
  if (end == NULL) end = strchr(start, '}');
  if (end == NULL) end = strchr(start, ']');
  if (end == NULL) return NULL;   // badly terminated

  // allocate storage for the result and copy it in
  char *result = new char[end-start+1];
  strncpy(result, start, end-start);
  result[end-start] = '\0';
  return result;
}


// function to check a character against a string of accepted characters
// returns true if the character is found in the string
static int chrstr(const char ch, const char *str)
{
  while (*str)
  {
    if (*(str++) == ch)
      return 1;
  } // while
  return 0;
}


static int contains_function(const char *str)
{
  for (int i = 0; function_names[i] != NULL; i++)
  {
    if (strstr(str, function_names[i]))
        return 1; // passed
  } // for i
  return 0; // failed
}


// check whether str is one of the valid choices in the keyword array
static int is_valid_keyword(const char *json_name,
                            const char *json_value,
                          const char *keywords[],
                          const int ignore_case = 0)
{
  for (int i = 0; keywords[i] != NULL; i++)
  {
    if (ignore_case)
    {
      if (!strcasecmp(json_value, keywords[i]))
        return 1; // passed
    }
    else
    {
      if (!strcmp(json_value, keywords[i]))
        return 1; // passed
    }
  } // for i

  // str failed to match any of the keyword choices
  fprintf(stderr, "validator: <%s> [%s] is not a valid choice\n", json_name, json_value);
  fprintf(stderr, "validator: <%s> Valid choices are: ", json_name);
  for (int j = 0; keywords[j] != NULL; j++)
  {
    fprintf(stderr, "[%s]", keywords[j]);
  } // for j
  fprintf(stderr, "\n");
  return 0; // failed
}


// Check the format of the version string matches the pattern 1.2.3.4
static int is_valid_version(const char *json_name, const char *json_value)
{
  int result = 1;
  char ch;

  int len = strlen(json_value);
  if (len < 7)
  {
    fprintf(stderr, "validator: %s must be at least 7 chars long.  Actual %d\n", json_name, len);
    return 0;
  }

  for (int i = 0; (ch = json_value[i]) != '\0'; i++)
  {
    if (isdigit(ch))
      continue;

    if (ch == '.')
      continue;

    // if the char isn't one of the ones we allow, report it as illegal
    if (isprint(ch))
    {
      fprintf(stderr, "validator: %s contains the illegal character [%c] in position %d\n", json_name, ch, i+1);
    }
    else
    {
      fprintf(stderr, "validator: %s contains an illegal unprintable character in position %d\n", json_name, i+1);
    }

    result = 0;
  }  // for i

  return result;
}


// checks the format of a GUID string
// expected format is "12345678-1234-1234-1234-123456789abc" All numbers are hex
static int is_guid(const char *str, const char *strname)
{
  int result = 1; // assume string is good until we find a bad char
  char ch;

  if (str == NULL)
  {
    fprintf(stderr, "validator: <%s> GUID is NULL\n", strname);
    return 0; // failed
  }
  int len = strlen(str);
  if (len != 36) // 8 + 4 + 4 + 4 + 12 = 32 + 4 dashes = 36
  {
    fprintf(stderr, "validator: <%s> GUID is incorrect length.  Expecting: 36 Actual: %d\n",
      strname, len);
    result = 0;
  }

  // look at each character in turn to see if it is correct.
  for (int i = 0; (ch = str[i]) != '\0'; i++)
  {
    if (i == 8 || i == 13 || i == 18 | i == 23)
    {
      if (ch != '-')
      {
        fprintf(stderr, "validator: <%s> Illegal character in GUID at position %d.  Expected [-] Got [%c]\n", strname, i+1, ch);
        result = 0;
      }
    }
    else if (!isxdigit(ch))
    {
      fprintf(stderr, "validator: <%s> Illegal HEX number [%c] in GUID at position %d.\n", strname, ch, i+1);
      result = 0;
    }
  } // for
  return result;
}


// The Scope value is dynamic so it might only contain a $env magic word
// But if it contains a static value, the format is either o/ or s/ followed by a GUID
// or p/ or a/ followed by a multi-character number  (ie p/12345)
static int is_valid_scope(const char *json_name, const char *json_value)
{
  if (!strncmp(json_value, "o/", 2) || !strncmp(json_value, "s/", 2))
    return is_guid(json_value+2, json_name);
  else if (!strncmp(json_value, "p/", 2) || !strncmp(json_value, "a/", 2))
  {
    int result = 1;
    char ch;
    for (int i = 2; (ch = json_value[i]) != '\0'; i++)
    {
      if (!isdigit(ch))
      {
        fprintf(stderr, "validator: <%s> Non numeric character [%c] at position %d.\n", json_name, ch, i+1);
        result = 0;
      }
    } // for
    return result;
  }
  else
  {
      fprintf(stderr, "validator: <%s> Illegal Format.  Must start with o/ s/ p/ or a/\n", json_name);
      return 0;
  }
}


// check that each character in a string is allowed
// if an illegal character is found, print an error and keep scanning
static int is_valid_string(const char *json_name,
                            const char *json_value,
                            const manifest_value_type value_type)
{
  int result = 1; // assume string is good until we find a bad char
  char ch;
  int at_count = 0; // count the @ in an email address

  // look at each character in turn to see if it is allowed.
  for (int i = 0; (ch = json_value[i]) != '\0'; i++)
  {
    if (islower(ch) || isdigit(ch) || ch == '-')
      continue;

    if (value_type != STRICT && isupper(ch))
      continue;

    if (value_type == EMAIL)
    {
      if (chrstr(ch, VALID_EMAIL_CHARACTERS))
        continue;

      if (ch == '@')
      {
        if (++at_count == 1)
          continue;

        // must mean that there are at least 2 @ in the email address
        fprintf(stderr, "validator: <%s> eMail address should only contain one '@'\n", json_name);
        result = 0; // failed
        continue;
      }
    }
    else if (value_type == URL)
    {
      if (chrstr(ch, VALID_URL_CHARACTERS))
        continue;
    }

    // if the char isn't one of the ones we allow, report it as illegal
    if (isprint(ch))
      fprintf(stderr, "validator: <%s> Value string contains the illegal character [%c] in position %d\n", json_name, ch, i+1);
    else
      fprintf(stderr, "validator: <%s> Value string contains the illegal unprintable character %02X in position %d\n", json_name, ch, i+1);
    result = 0; //failed
  } // for i

  if (value_type == EMAIL && at_count == 0)
  {
    fprintf(stderr, "validator: <%s> eMail address does not contain '@' character\n", json_name);
    result = 0; //failed
  }

  return result;
}


// Enforce the basic requirements for fields which make up the CRN
// ie must not contain :
static int is_valid_crn_field(const char *json_name, const char *json_value)
{
  int result = 1; // assume string is good until we find a bad char
  char ch;

  for (int i = 0; (ch = json_value[i]) != '\0'; i++)
  {
    if (ch == ':')
    {
      fprintf(stderr, "validator: <%s> CRN field has illegal character [%c] in position %d\n", json_name, ch, i+1);
      result = 0;
    }
  } // for

  return result;
}


int main(int argc, char **argv)
{
  const char *copyright_str = "© Copyright 2016,2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM";

  printf("Service Manifest JSON validator v1.1\n");
  printf("%s\n", copyright_str);

  int manifest_json_len = manifest_data_end - manifest_data;
  char *manifest_json = new char[manifest_json_len + 1];
  strncpy(manifest_json, manifest_data, manifest_json_len);
  manifest_json[manifest_json_len] = '\0';

  int result = 0;  // Defaults to success unless we find a failure

  for(int i = 0; manifest_elements[i].name != NULL; i++)
  {
    const char *name = manifest_elements[i].name;
    char *value = find_json_value(manifest_json, name);
    if (value == NULL)
    {
      fprintf(stderr, "validator: <%s> Field cannot be found\n", name);
      result = 1;
      continue;
    }
    else if (strlen(value) == 0)
    {
      if (manifest_elements[i].is_required)
      {
        fprintf(stderr, "validator: <%s> is a required field but the value is EMPTY\n", name);
        result = 1;
      }
      delete[] value;
      continue;
    }

    // if there is a function in the value then there is no point checking further
    // because the true value wont be known until deployment time
    if (manifest_elements[i].allow_func && contains_function(value))
    {
      delete[] value;
      continue;
    }

    // perform an extra check on the CRN fields as well as the other checks
    if (manifest_elements[i].is_crn)
    {
      if (!is_valid_crn_field(name, value)) result = 2;
    }

    switch (manifest_elements[i].value_type) {
      case VERSION:
        if (!is_valid_version(name, value)) result = 2;

        break;
      case STRICT:
      case EMAIL:
      case URL:
        if (!is_valid_string(name, value, manifest_elements[i].value_type)) result = 2;
        break;

      case KEYWORD:
        if (!is_valid_keyword(name, value, manifest_elements[i].keyword_choices)) result = 2;
        break;

      case SCOPE:
        if (!is_valid_scope(name, value)) result = 2;
        break;

      case BASIC:
        // todo: is there anything more I can check for these guys?
        break;

      default:
        printf("<%s> Type not implemented yet\n", name);
    } // switch

    delete[] value;
  } /// for

  delete[] manifest_json;

  if (result != 0)
  {
    fprintf(stderr, "validator: *** Value validation FAILED\n\n");
    return result;
  }

  printf("All tests PASSED\n\n");
  return result;
}
