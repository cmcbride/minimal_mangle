/* uber-simplified C-only code utilities to use mangle polygons
 *
 * Cameron McBride
 * cameron.mcbride AT gmail.com
 * January 2013
 */
#pragma once
#ifndef MANGLELIB_INCLUDED
#define MANGLELIB_INCLUDED

/*
 * Some description:
 *   The main focus of these utilites:
 *   1. clean c-only code
 *   2. simple use
 *   3. easy to use within larger codes
 *
 *   There is no attempt to provide complete functionality.
 *   that MANGLE does.  Please use full MANGLE utilties if you need
 *   to convert to an accepted polygon format.
 **/

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <check_alloc.c>
#include <check_fopen.c>
#include <simple_reader.c>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef PI
#define PI (4.0 * atan(1.0))
#endif

typedef int SINT;               /* SINT = signed integer */

typedef struct {
    double x[3];
} VEC3;

typedef struct {
    double x[3];
    double m;
} CAP;

typedef struct {
    SINT polyid;
    SINT pixel;
    SINT ncap;
    CAP *cap;
    double weight;
    double area;
} POLY;

/* this is a linked-list for IDs, intended for pixels  */
typedef struct {
    SINT id;
    void *next;
} ID_LIST;

typedef struct {
    SINT npoly;
    POLY *poly;
    SINT pix_r;
    ID_LIST *pix;               /* pixel-indexed array of linked-lists */
} PLY;

void
ply_poly_init( POLY * p, const SINT polyid, const SINT ncap,
               const double weight, const SINT pixel, const double area )
{
    p->polyid = polyid;
    p->cap = check_alloc( ncap, sizeof( CAP ) );
    p->ncap = ncap;
    p->weight = weight;
    p->pixel = pixel;
    p->area = area;
}

void
ply_poly_clean( POLY * p )
{
    p->polyid = -1;
    CHECK_CLEAN( p->cap );
    p->ncap = 0;
    p->weight = 0.0;
    p->pixel = -1;
    p->area = 0.0;
}

void
ply_init( PLY * ply, SINT npoly )
{
    if( npoly > 0 ) {
        ply->poly = check_alloc( npoly, sizeof( POLY ) );
    } else {
        ply->poly = NULL;
    }
    ply->npoly = npoly;
    ply->pix_r = 0;
}

void
ply_clean( PLY * ply )
{
    SINT i;
    POLY *p;
    for( i = 0; i < ply->npoly; i++ ) {
        p = &( ply->poly[i] );
        ply_poly_clean( p );
    }
    CHECK_CLEAN( ply->poly );
    ply->npoly = 0;
    ply->pix_r = 0;
}

PLY *
ply_create( SINT npoly )
{
    PLY *ply;
    ply = check_alloc( 1, sizeof( PLY ) );
    ply_init( ply, npoly );
    return ply;
}

PLY *
ply_destroy( PLY * ply )
{
    ply_clean( ply );
    CHECK_CLEAN( ply );
    return NULL;
}

void
ply_pix_init( PLY * ply, SINT pix_r )
{
    /* use pix_r to allocate ply->pix array */
}

void
ply_file_read( PLY * ply, const char const *filename )
{
    /* read in polygon format */
    int check;
    int npoly = 0;
    SINT ipoly = 0;
    simple_reader *sr;
    char *line;

    ply_clean( ply );

    /* read-by-line and process PLY format file */
    sr = sr_create( filename );

    /* first line sets up the polygons */
    line = sr_readline( sr );
    check = sscanf( line, "%d polygons", &npoly );
    if( check != 1 || npoly < 1 ) {
        fprintf( stderr,
                 "MANGLE_LIB Error: polygons (%d) must be positive in file: %s\n",
                 npoly, sr_filename( sr ) );
        exit( EXIT_FAILURE );
    }

    ply_init( ply, npoly );

    ipoly = 0;
    while( sr_readline( sr ) ) {
        int i, polyid, ncap, pixel;
        double weight, area;
        POLY *p;

        line = sr_line( sr );

        if( strncmp( "polygon", line, 7 ) == 0 ) {
            check =
                sscanf( line,
                        "polygon %d ( %d caps, %lf weight, %d pixel, %lf",
                        &polyid, &ncap, &weight, &pixel, &area );
            if( check != 5 || ncap < 1 ) {
                fprintf( stderr,
                         "MANGLE_LIB Error: polygon read error line %d in file: %s\n",
                         sr_linenum( sr ), sr_filename( sr ) );
                exit( EXIT_FAILURE );
            }

            if( ipoly >= ply->npoly ) {
                fprintf( stderr,
                         "MANGLE_LIB Error: too many polygons on line %d in file: %s\n",
                         sr_linenum( sr ), sr_filename( sr ) );
                exit( EXIT_FAILURE );
            }

            /* we're starting a valid polygon! */
            p = &ply->poly[ipoly];
            ply_poly_init( p, polyid, ncap, weight, pixel, area );
            for( i = 0; i < ncap; i++ ) {
                CAP *c;
                c = &p->cap[i];
                line = sr_readline( sr );
                check = sscanf( line, "%lf %lf %lf %lf", &c->x[0], &c->x[1], &c->x[2], &c->m );
                if( check != 4 ) {
                    fprintf( stderr,
                             "MANGLE_LIB Error: cap read error on line %d in file: %s\n",
                             sr_linenum( sr ), sr_filename( sr ) );
                    exit( EXIT_FAILURE );
                }
            }
            ipoly += 1;
        }
        /* pretty much ignore anything else, but will implement simple pixelization */
    }

    sr_destroy( sr );
}

PLY *
ply_from_file( const char const *filename )
{
    PLY *ply;
    ply = ply_create( 0 );
    ply_file_read( ply, filename );
    return ply;
}

/* VEC3: this can be abstracted: a calling code can just use (void *) */
VEC3 *
ply_vec_create( void )
{
    VEC3 *vec3;
    vec3 = check_alloc( 1, sizeof( VEC3 ) );
    return vec3;
}

VEC3 *
ply_vec_destroy( VEC3 * vec3 )
{
    CHECK_CLEAN( vec3 );
    return vec3;
}

/* convert polor sky coordinates to unit vector:
 * el = elevation / polar
 * az = azimuthal
 */
static inline void
ply_vec_from_polar( VEC3 * vec3, const double az, const double el )
{
    vec3->x[0] = sin( el ) * cos( az );
    vec3->x[1] = sin( el ) * sin( az );
    vec3->x[2] = cos( el );
}

static inline void
ply_vec_from_radec_deg( VEC3 * vec3, const double ra, const double dec )
{
    double az = PI / 180.0 * ra;
    double el = PI / 180.0 * ( 90.0 - dec );
    ply_vec_from_polar( vec3, az, el );
}

static inline int
ply_within_cap( const CAP const *cap, const VEC3 const *vec3 )
{
    const double *c;
    const double *v;
    c = cap->x;
    v = vec3->x;
    double cd = 1.0 - c[0] * v[0] - c[1] * v[1] - c[2] * v[2];
    if( cap->m < 0.0 ) {
        if( cd > fabs( cap->m ) )
            return TRUE;
    } else {
        if( cd < cap->m )
            return TRUE;
    }

    return FALSE;
}

static inline int
ply_within_poly( const POLY const *p, const VEC3 const *vec3 )
{
    SINT i;
    CAP *c;
    c = p->cap;
    for( i = 0; i < p->ncap; i++ ) {
        if( !ply_within_cap( &c[i], vec3 ) )
            return FALSE;
    }
    return TRUE;
}

static inline int
ply_polyid_first( const PLY const *ply, const VEC3 const *vec3 )
{
    SINT i;
    POLY *p;

    for( i = 0; i < ply->npoly; i++ ) {
        p = &( ply->poly[i] );
        if( ply_within_poly( p, vec3 ) )
            return p->polyid;
    }
    return -1;
}

#endif
