#include <stdlib.h>
#include <stdio.h>

#include <minimal_mangle.c>

int
main( int argc, char **argv )
{
    MANGLE_PLY *ply;
    MANGLE_INT i, sid;
    size_t npix, count;

    if( argc < 2 ) {
        printf( "Usage: %s  POLYGON  >  OUTPUT\n", argv[0] );
        return EXIT_FAILURE;
    }
    ply = mply_read_file( argv[1] );

    npix = mply_pix_count( ply->pix_res );
    sid = mply_pix_id_start( ply );

    fprintf( stderr, "Sky pixelized into %zd pixels", npix );
    if( npix > 0 ) {
        fprintf( stderr, " (IDs: %d - %zd)\n", sid, sid + npix - 1 );
    } else {
        fprintf( stderr, "\n" );
    }

    /* starting pixel ID */
    for( i = 0; i < npix; i++ ) {
        count = mply_pix_npoly( ply, i );
        if( count > 0 ) {
            fprintf( stdout, "%6d %6d %6zd\n", i, i + sid, count );
        }
    }

    ply = mply_kill( ply );

    return EXIT_SUCCESS;
}
