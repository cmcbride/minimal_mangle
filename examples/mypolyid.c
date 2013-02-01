#include <stdlib.h>
#include <stdio.h>

#include <minimal_mangle.c>
#include <simple_reader.c>

int
main( int argc, char **argv )
{
    PLY *ply;
    simple_reader *sr;
    void *vec;

    if( argc < 3 ) {
        printf( "Usage: %s  POLYGON  RA_DEC_FILE\n", argv[0] );
        return EXIT_FAILURE;
    }
    ply = ply_read_file( argv[1] );
    sr = sr_init( argv[2] );
    vec = ply_vec_init(  );

    while( sr_readline( sr ) ) {
        char *line;
        int check, polyid;
        double ra, dec;

        line = sr_line( sr );

        if( '#' == line[0] )
            continue;
        if( '\0' == line[0] )
            continue;

        check = sscanf( line, "%lf %lf", &ra, &dec );
        if( 2 != check ) {
            fprintf( stderr, "WARNING: skipped line, couldn't read RA/DEC on line %d in file %s\n",
                     sr_linenum( sr ), sr_filename( sr ) );
            continue;
        }

        ply_vec_from_radec_deg( vec, ra, dec );
        polyid = ply_polyid_first( ply, vec );

        fprintf( stdout, "%8d %s\n", polyid, line );
    }

    vec = ply_vec_kill( vec );
    ply = ply_kill( ply );
    sr = sr_kill( sr );

    return EXIT_SUCCESS;
}
