#include <stdlib.h>
#include <stdio.h>

#include <minimal_mangle.c>
#include <simple_reader.c>

int
main( int argc, char **argv )
{
    /* these could also be declared as "void *" */
    PLY *ply;
    void *vec;
    simple_reader *sr;

    double min_weight = 0.0;

    if( argc < 3 ) {
        printf( "Usage: %s  POLYGON  RA_DEC_FILE  [MIN_WEIGHT]\n", argv[0] );
        return EXIT_FAILURE;
    }

    fprintf( stderr, "Reading polygon file: %s\n", argv[1] );
    ply = ply_read_file( argv[1] );

    if( argc > 3 ) {
        min_weight = strtod( argv[3], NULL );
    }

    fprintf( stderr, "Filtering: weight >= %g\n", min_weight );

    vec = ply_vec_init(  );     /* vector to store search position */
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

        ply_vec_from_radec_deg( vec, ra, dec );
        index = ply_find_index( ply, vec );

        if( index < 0 )
            continue;

        weight = ply_weight_from_index( ply, index );
        if( weight < min_weight )
            continue;

        fprintf( stdout, "%s\n", line );
    }

    vec = ply_vec_kill( vec );
    ply = ply_kill( ply );
    sr = sr_kill( sr );

    return EXIT_SUCCESS;
}
