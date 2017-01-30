/*
Syntax colorizer :)
*/

#include <stdio.h>
#include <string.h>

// auxilary functions :)
void write_converted( FILE* file, int character );
int isKeyword( char* str );
void add_letter_to_string( char letter, char* str );
void flush_string( FILE* file, char* str );

int main( int argc, char** argv )
{
	FILE* src;
	FILE* dest;
	char aux[1024];
	int aux2;
	int character = 0;
	int prev_character = 0;

	if( argc != 3 )
	{
		printf("Invalid parameter count.");
		return 1;
	}

	src = fopen( argv[1], "r" );
	dest = fopen( argv[2], "w" );

	fprintf( dest, "<html><head><title>%s</title></head><body><code>", argv[1] );

	*aux = 0;
	while( !feof( src ) )
	{
		prev_character = character;
		character = fgetc( src );

		if( ( ( character >= '0' ) && ( character <= '9' ) ) ||
			( ( character >= 'A' ) && ( character <= 'Z' ) ) ||
			( ( character >= 'a' ) && ( character <= 'z' ) ) )
		{
			add_letter_to_string( character, aux );
			continue;
		}
		else
		{
			if( isKeyword( aux ) )
			{
				fprintf( dest, "<b><i>" );
				aux2 = 1;
			}
			flush_string( dest, aux );
			if( aux2 == 1 )
			{
				fprintf( dest, "</i></b>" );
				aux2 = 0;
			}
		}

		if( character == '\'' )
		{
			fprintf( dest, "<font color=\"red\">" );
			do
			{
				write_converted( dest, character );
				prev_character = character;
				character = fgetc( src );
				if( prev_character == '\\' )
				{
					write_converted( dest, character );
					prev_character = character;
					character = fgetc( src );
				}
			} while( character != '\'' );
			fprintf( dest, "\'</font>" );
			continue;
		}

		if( character == '"' )
		{
			fprintf( dest, "<font color=\"bronze\">" );
			do
			{
				write_converted( dest, character );
				prev_character = character;
				character = fgetc( src );
				if( prev_character == '\\' )
				{
					write_converted( dest, character );
					prev_character = character;
					character = fgetc( src );
				}
			} while( character != '"' );
			fprintf( dest, "\"</font>" );
			continue;
		}

		if( ( character == '#' ) && ( ( prev_character == 0 ) || ( prev_character == '\n' ) ) )
		{
			fprintf( dest, "<font color=\"blue\">" );
			while( character != '\n' )
			{
				write_converted( dest, character );
				prev_character = character;
				character = fgetc( src );
			}
			fprintf( dest, "<br></font>" );
			continue;
		}

		if( character == '/' )
		{
			prev_character = character;
			character = fgetc( src );

			if( character == '*' )
			{
				fprintf( dest, "<font color=\"green\">/" );
				while( !( ( character == '/' ) && ( prev_character == '*' ) ) )
				{
					write_converted( dest, character );
					prev_character = character;
					character = fgetc( src );
				}
				fprintf( dest, "/</font>" );
				continue;
			}
			if( character == '/' )
			{
				fprintf( dest, "<font color=\"green\">/" );
				while( character != '\n' )
				{
					write_converted( dest, character );
					prev_character = character;
					character = fgetc( src );
				}
				fprintf( dest, "<br></font>" );
				continue;
			}

			write_converted( dest, prev_character );
		}

		write_converted( dest, character );
	}

	fprintf( dest, "</code></body></html>" );

	fclose( src );
	fclose( dest );

	return 0;
}

void write_converted( FILE* file, int character )
{
	switch( character )
	{
		case '<':
			fprintf( file, "&lt;" );
			break;
		case '>':
			fprintf( file, "&gt;" );
			break;
		case '&':
			fprintf( file, "&amp;" );
			break;
		case '\t':
			fprintf( file, "&nbsp;&nbsp;&nbsp;&nbsp;" );
			break;
		case '\n':
			fprintf( file, "<br>" );
			break;
		default:
			fputc( character, file );
	}
}

int isKeyword( char* str )
{
	int i;
	char keywords[][20] =
	{
		"auto",
		"break",
		"case",
		"char",
		"const",
		"continue",
		"default",
		"do",
		"double",
		"else",
		"enum",
		"extern",
		"float",
		"for",
		"goto",
		"if",
		"int",
		"long",
		"register",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"struct",
		"switch",
		"typedef",
		"union",
		"unsigned",
		"void",
		"volatile",
		"while",
		""
	};

	i = 0;
	while( strcmp( keywords[i], "" ) )
	{
		if( !strcmp( str, keywords[i] ) )
			return 1;
		++i;
	}

	return 0;
}

void add_letter_to_string( char letter, char* str )
{
	while( *str )
		++str;
	*str = letter;
	++str;
	*str = 0;
}

void flush_string( FILE* file, char* str )
{
	while( *str )
	{
		write_converted( file, *str );
		*str = 0;
		++str;
	}
}
