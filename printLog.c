/*
 *  「しまにてぃ」の状況報告を標準出力に出力する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/printLog.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 1     06/09/15 16:17 Tsujimura543
 * deleteMod(), logBeautify(), printLog(), printTracerLog() を simanity.c
 * から分離し、printLog.c を作成
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/printLog.c 1     09/05/14 4:45 tsupo $";
#endif

char    *
deleteMod( char *log, const char *mod )
{
    char    *v, *w;
    char    target[16];
    size_t  len;

    sprintf( target, ":%s:", mod );
    len = strlen( target );

    v = log;
    while ( (w = strstr( v, target )) != NULL )
        strcpy( w, w + len );

    return ( log );
}

char    *
logBeautify( char *log,
             long *targetIslandID,
             char *targetIslandName )
{
    const char  *p, *q, *r;
    char        target[BUFSIZ];

    p = strstr( log, ":na1:" );
    if ( p ) {
        if ( *targetIslandID == 0 )
            *targetIslandID = atol( p + 5 );
        q = strstr( p + 5, ":na2:" );
        if ( q ) {
            p++;
            q += 4;
            strncpy( target, p, q - p );
            target[q - p] = NUL;

            q++;
            if ( targetIslandName[0] == NUL ) {
                r = strstr( q, "島:p:" );
                if ( r ) {
                    strncpy( targetIslandName, q, r - q );
                    targetIslandName[r - q] = NUL;
                }
            }

            deleteMod( log, target );
        }
        else
            deleteMod( log, "na1" );
    }
    else
        deleteMod( log, "na2" );

    deleteMod( log, "p" );
    deleteMod( log, "n" );
    deleteMod( log, "c" );
    deleteMod( log, "o" );
    deleteMod( log, "b" );
    deleteMod( log, "r" );

    return ( log );
}

void
printLog( const char *log,
          long *targetIslandID,
          char *targetIslandName )
{
    const char  *v;

    v = strchr( log, '"' );
    if ( v ) {
        char    temp2[BUFSIZ];

        strncpy( temp2, log, v - log );
        temp2[v - log] = NUL;
        logBeautify( temp2, targetIslandID, targetIslandName );
        printf( "  %s\n", temp2 );
    }
}

void
printTargetLog( char *log, const char *targetIslandName )
{
    long    dummyID    = 1;
    char    *dummyName = "dummy";

    logBeautify( log, &dummyID, dummyName );
    printf( "  %s島: %s\n", targetIslandName, log );
}

