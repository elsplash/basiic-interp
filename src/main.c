/*
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This is a shell implementation of the library I
wrote.

This will simply recreate the environment in a
local folder, and will not go out of that folder
for your safety.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#define BASIIC_IMPL
#include <basiic.h>

#define BASIIC_CSHELL_VER "v0.0.1_alpha build 10212026"
#define BASIIC_CSHELL_BUFFER_SZ 128

int main(const int argc, const char **argv)
{
  char buffer[BASIIC_CSHELL_BUFFER_SZ];
  puts("BASIIC CShell\n"
       "A simple qBasic emulator\n\n"

       BASIIC_CSHELL_VER "\n\n"

       "You may enter `HELP` for a basic guide.\n\n");

  spans_ls_table_t lt = spans_ls_table_creat();

  while (1)
  {
    puts("READY.\n\n");
    printf("> ");
    fgets(buffer, BASIIC_CSHELL_BUFFER_SZ, stdin);
    spans_linespan_t ls = spans_ls_init(buffer, NULL, lt.len);
    spans_ls_table_push(&lt, ls);

    // ...
  }

  spans_ls_table_free(&lt);
  return 0;
}
