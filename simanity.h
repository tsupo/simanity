/*
 *  「しまにてぃ」の自分の島の現在の状況を取得、表示する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/simanity.h $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 2     06/09/15 16:18 Tsujimura543
 * printLog(), printTracerLog() のプロトタイプ宣言を追加
 * 
 * 1     06/09/15 15:26 Tsujimura543
 * main(), usage(), setParam() を simanity.c から分離し、main.c を作成した
 * のに伴い、 main.c  内から参照している関数のプロトタイプ宣言を集めて、本
 * ヘッダファイルを作成した
 */

#ifndef __SIMANITY_H__
#define __SIMANITY_H__

BOOL
loginSimanity( const char *username, const char *password,
               char *cookie, char *simaID, char *hash,
               char *request,
               char *response, size_t sz );

void
visitSima( const char *simaID, const char *hash,
           char *cookie, char *src,
           char *request,
           char *response, size_t sz );

void
getArea( const char *src,
         char *cookie, char *area,
         char *response, size_t sz );

void
getMapURL( const char *area,
           char *cookie, char *dst,
           char *request,
           char *response, size_t sz );

char    *
getReport( const char *dst,
           char *cookie,
           char *pass,
           char *response, size_t sz );

void
printReport( const char *area,
             const char *simaID,
             const char *pass,
             const char *response,
             char       *cookie );


void
printLog( const char *log,
          long *targetIslandID,
          char *targetIslandName );

void
printTargetLog( char *log,
                const char *targetIslandName );


#endif  /* __SIMANITY_H__ */
