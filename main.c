/*
 *  「しまにてぃ」の自分の島の現在の状況を取得、表示する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/main.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 1     06/09/15 15:24 Tsujimura543
 * main(), usage(), setParam() を simanity.c から分離し、main.c を作成
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/main.c 1     09/05/14 4:45 tsupo $";
#endif

void
usage( void )
{
    fprintf( stderr, "「しまにてぃ」の自分の島の状況を取得する\n" );
    fprintf( stderr,
             "    simanity [-x] [-u メールアドレス] [-p パスワード]\n" );
    fprintf( stderr, "\t-x: proxy を使う\n" );
}

void
setParam(
        int  argc,
        char *argv[],
        char *username,
        char *password,
        BOOL *useProxy
    )
{
    int i, j;

    for ( i = 1; i < argc; i++ ) {
        if ( argv[i][0] == '-' ) {
            for ( j = 1; argv[i][j]; j++ ) {
                switch ( argv[i][j] ) {
                case 'u':
                    if ( argv[i][j + 1] ) {
                        strcpy( username, &argv[i][j + 1] );
                        j = (int)strlen( argv[i] ) - 1;
                    }
                    else if ( i + 1 < argc ) {
                        i++;
                        strcpy( username, argv[i] );
                        j = (int)strlen( argv[i] ) - 1;
                    }
                    break;

                case 'p':
                    if ( argv[i][j + 1] ) {
                        strcpy( password, &argv[i][j + 1] );
                        j = (int)strlen( argv[i] ) - 1;
                    }
                    else if ( i + 1 < argc ) {
                        i++;
                        strcpy( password, argv[i] );
                        j = (int)strlen( argv[i] ) - 1;
                    }
                    break;

                case 'x':
                    *useProxy = !(*useProxy);
                    break;
                }
            }
        }
    }
}

int
main( int argc, char *argv[] )
{
    int     ret = 0;
    BOOL    useProxy = FALSE;
    char    username[MAX_USERIDLEN];
    char    password[MAX_PASSWDLEN];
    char    cookie[MAX_COOKIE_LEN];
    char    *request;
    char    *response;
    size_t  sz = MAX_CONTENT_SIZE * 20;

    char    simaID[32];
    char    hash[64];
    char    src[MAX_URLLENGTH];
    char    dst[MAX_URLLENGTH];
    char    area[16];
    BOOL    stat;

    if ( argc <= 1 ) {
        usage();
        return ( ret );
    }

    memset( username, 0x00, MAX_USERIDLEN );
    memset( password, 0x00, MAX_PASSWDLEN );

    if ( argc >= 2 )
        setParam( argc, argv,
                  username, password, &useProxy );

    if ( !(username[0]) || !(password[0]) )
        return ( ret );

    cookie[0] = NUL;
    simaID[0] = NUL;
    hash[0]   = NUL;
    src[0]    = NUL;
    dst[0]    = NUL;
    area[0]   = NUL;

    request  = (char *)malloc( MAX_CONTENT_SIZE );
    response = (char *)malloc( sz );
    if ( !request || !response )
        return ( ret );

    if ( useProxy )
        setUseProxy( useProxy );

    // ログインする
    stat = loginSimanity( username, password,
                          cookie, simaID, hash,
                          request,
                          response, sz );

    if ( stat ) {
        // しまを見る
        if ( simaID[0] && hash[0] )
            visitSima( simaID, hash,
                       cookie, src,
                       request,
                       response, sz );

        // 海域を取得する
        if ( src[0] )
            getArea( src,
                     cookie, area,
                     response, sz );

        // マップを見るための URL を取得する
        if ( area[0] )
            getMapURL( area,
                       cookie, dst,
                       request,
                       response, sz );

        // マップを見る (自分の島の状況を見る)
        if ( dst[0] ) {
            char    pass[32];
            getReport( dst, cookie, pass, response, sz );
            if ( *response )
                printReport( area, simaID, pass,
                             response,
                             cookie );
        }
    }

    free( response );
    free( request );

    return ( ret );
}
