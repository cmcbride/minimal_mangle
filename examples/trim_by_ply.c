#include <stdlib.h>
#include <stdio.h>

#include <minimal_mangle.c>
#include <simple_reader.c>

int
main( int argc, char **argv )
{
    /* these could also be declared as "void *" */
    MANGLE_PLY *ply;
    simple_reader *sr;

    double min_weight = 0.0;

    if( argc < 3 ) {
        printf( "Usage: %s  POLYGON  RA_DEC_FILE  [MIN_WEIGHT]\n", argv[0] );
        return EXIT_FAILURE;
    }

    fprintf( stderr, "Reading polygon file: %s\n", argv[1] );
    ply = mply_read_file( argv[1] );

    if( argc > 3 ) {
        min_weight = strtod( argv[3], NULL );
    }

    fprintf( stderr, "Filtering: weight >= %g\n", min_weight );

    sr = sr_init( argv[2] );    /* simple line-by-line reader */
    while( sr_readline( sr ) ) {
        char *line;
        int check;
        MANGLE_INT index;
        double ra, dec, weight;

        line = sr_line( sr );

        if( sr_line_isempty( sr ) )
            continue;
        if( '#' == line[0] )
            continue;

        check = sscanf( line, "%lf %lf", &ra, &dec );
        if( 2 != check ) {
            fprintf( stderr, "WARNING: skipped line, couldn't read RA/DEC on line %d in file %s\n",
                     sr_linenum( sr ), sr_filename( sr ) );
            continue;
        }

        index = mply_find_polyindex_radec( ply, ra, dec );

        if( index < 0 )
            continue;

        weight = mply_weight_from_index( ply, index );
        if( weight < min_weight )
            continue;

        fprintf( stdout, "%s\n", line );
    }

    ply = mply_kill( ply );
    sr = sr_kill( sr );

    return EXIT_SUCCESS;
}
