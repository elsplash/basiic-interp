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

#define BASIIC_CSHELL_VER "v26.01_alpha"

int main(const int argc, const char **argv)
{
  puts("BASIIC CShell\n"
       "A simple qBasic emulator\n\n"

       BASIIC_CSHELL_VER "\n\n"

       "You may enter `HELP` for a basic guide.\n\n");

  while (1)
  {
    printf("> ");
    fgets(buffer, stdin, 32);
  }
  return 0;
}
