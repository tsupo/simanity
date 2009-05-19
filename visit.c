/*
 *  「しまにてぃ」の「しまを見る」に相当する一連の http リクエストを実行する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/visit.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 2     07/03/29 18:39 Tsujimura543
 * 2007年3月27日の「しまにてぃ」のメンテナンスで仕様に変更があった部分に
 * 対応するための修正を実施
 * 
 * 1     06/09/15 16:03 Tsujimura543
 * visitSima(), getArea(), getMapURL() を simanity.c から分離し、visit.c
 * を作成
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/visit.c 1     09/05/14 4:45 tsupo $";
#endif


/* 注: 送信電文格納領域、受信電文格納領域は、以下の各関数で使いまわすことを */
/*     想定している (領域は各関数の呼び出し元であらかじめ確保しておくこと)  */

void
visitSima(
        const char *simaID,     /* (I)   しまにてぃID (ユーザID)            */
        const char *hash,       /* (I)   しまにてぃIDに紐付けられたハッシュ */
        char       *cookie,     /* (I/O) クッキー                           */
        char       *src,        /* (O)   次にアクセスすべき URL             */
        char       *request,    /* (I/O) 送信電文格納領域                   */
        char       *response,   /* (I/O) 受信電文格納領域                   */
        size_t     sz           /* (I)   受信電文格納領域サイズ             */
    )
{
    char    url[MAX_URLLENGTH];

    setUpReceiveBuffer( response, sz );
    sprintf( request,
             "p=h_home&"
             "c_member_id=%s&"
             "user_hash=%s&"
             "game_type=0",
             simaID,    // しまにてぃID
             hash       // ハッシュ
           );
    strcpy( url, "http://g.sima.jp/page.php" );
    http_postEx( url, "application/x-www-form-urlencoded",
                 request, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, "<iframe src=\"" );
        if ( q ) {
            q += 13;
            r = strchr( q, '"' );
            if ( r ) {
                strncpy( src, q, r - q );
                src[r - q] = NUL;
            }
        }
    }
    /* いったん http://g.sima.jp/* にアクセスした後は、hash に相当する情報 */
    /* が cookie に格納されるので、以降のアクセスでは hash は不要になる    */
}

void
getArea(
        const char *src,        /* (I) visitSima() で取得した URL           */
        char       *cookie,     /* (I/O) クッキー                           */
        char       *area,       /* (O)   海域名                             */
                                /*         「次にアクセスすべき URL」を確定 */
                                /*         するのに必要                     */
        char       *response,   /* (I/O) 受信電文格納領域                   */
        size_t     sz           /* (I)   受信電文格納領域サイズ             */
    )
{
    setUpReceiveBuffer( response, sz );
    http_getEx( src, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, " name='area' value='" );
        if ( q ) {
            q += 20;
            r = strchr( q, '\'' );
            if ( r ) {
                strncpy( area, q, r - q );
                area[r - q] = NUL;
            }
        }
    }
}

void
getMapURL(
        const char *area,     /* (I)   getArea() で取得した「海域名」       */
        char       *cookie,   /* (I/O) クッキー                             */
        char       *dst,      /* (O)   自島情報を得るためにアクセスする URL */
        char       *request,  /* (I/O) 送信電文格納領域                     */
        char       *response, /* (I/O) 受信電文格納領域                     */
        size_t     sz         /* (I)   受信電文格納領域サイズ               */
    )
{
    char    url[MAX_URLLENGTH];

    strcpy( url, "http://g.sima.jp/cgi-bin/n/island.cgi" );
    sprintf( request,
             "s=r&"
             "area=%s&"
             "framemode=outer",
             area );
    setUpReceiveBuffer( response, sz );
    http_postEx( url, "application/x-www-form-urlencoded",
                 request, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, "CONTENT='1;URL=" );
        if ( q ) {
            q += 15;
            r = strchr( q, '\'' );
            if ( r ) {
                strncpy( dst, q, r - q );
                dst[r - q] = NUL;
            }
        }
    }
}

