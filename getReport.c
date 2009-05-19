/*
 *  「しまにてぃ」の「自分のしま」の現況を取得する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/getReport.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 1     06/09/15 16:51 Tsujimura543
 * getReport() を simanity.c から分離し、getReport.c を作成
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/getReport.c 1     09/05/14 4:45 tsupo $";
#endif

char    *
getReport(
        const char *dst,        /* (I)   しま情報アクセス URL の相対パス    */
        char       *cookie,     /* (I/O) クッキー                           */
        char       *pass,       /* (O)   しま情報アクセスに必要なパスワード */
        char       *response,   /* (I/O) 受信電文格納領域                   */
        size_t     sz           /* (I)   受信電文格納領域サイズ             */
    )
{
    char        url[MAX_URLLENGTH];
    const char  *p, *q;

    pass[0] = NUL;
    p = strstr( dst, "password=" );
    if ( p ) {
        p += 9;
        q = strchr( p, '&' );
        if ( q ) {
            strncpy( pass, p, q - p );
            pass[q - p] = NUL;
        }
    }

    sprintf( url, "http://g.sima.jp%s", dst );
    setUpReceiveBuffer( response, sz );
    http_getEx( url, response, cookie );

    return ( response );
}
