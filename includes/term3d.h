/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:58:31 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/21 11:01:27 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM3D_H
# define TERM3D_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <termios.h>
# include <ctype.h>
# include <sys/time.h>
# include <fcntl.h>
# include <sys/select.h>
# include <sys/ioctl.h>
# include <stdbool.h>
# include <float.h>

# define T3_MAX_WIDTH 350
# define T3_MAX_HEIGHT 100
# define T3_GLYPH_FILE "./printables.txt"
// グリフ総数
# define T3_GLYPH_NUM 96
// 1つのグリフの横幅(文字数)
# define T3_GLYPH_WIDTH 20
// 1つのグリフの縦幅(文字数)
# define T3_GLYPH_HEIGHT 40
# define T3_GLYPH_SCALE 0.1
# define T3_MAX_MSGLEN 20
// 画面クリア用エスケープシーケンス
# define T3_ES_CLEAR "\x1b[H"
// キー
# define T3_CHAR_ESC 27

typedef uint64_t	t_ut;

// 文字列におけるカーソルの位置
typedef size_t		t_position[2];

// 3次元ベクトル構造体
typedef double		t_vector3d[3];

// アフィン変換行列(4x4)
// a[16]でもいい？
typedef double		t_affine[4][4];

// グリフ(字形)
typedef struct s_glyph {
	size_t		n_points;
	t_vector3d	*points;
}	t_glyph;

typedef enum e_source {
	T3_SRC_DUMMY,
	T3_SRC_TEXT,
	T3_SRC_FILE_3D,
}	t_source;

// カメラパラメータ構造体
// sq_size:  スクエアサイズ
// offset:   オフセット
// 位置(-offset_x, -offset_y)が原点(0, 0)に来るように移動してからレンダリングする
// rot_axis: 回転軸
// phi:      回転角
// omega:    角速度、つまり 1秒あたりの回転角度[ラジアン/マイクロ秒]
// width:    横文字数
// height:   縦文字数
// pixels:   ピクセルバッファ
// uspf:     マイクロ秒毎フレーム
// animate:  時間変化するかどうかのフラグ
typedef struct s_optics {
	double		sq_size_x;
	double		sq_size_y;
	double		offset_x;
	double		offset_y;
	double		scale_factor;
	t_vector3d	rot_axis;

	double		phi;
	double		omega;

	size_t		width;
	size_t		height;
	size_t		pixels[T3_MAX_HEIGHT][T3_MAX_WIDTH];
	double		fps;
	t_ut		uspf;

	bool		animate;
}	t_optics;

// 系全体を表す構造体
// n_points:           点の数
// points_original:    点のデータ
// points_animated:    回転後の点のデータ
// 2つのpointsはどちらも同サイズで、どちらもmalloc-ed.
// transform_static:   回転直前までもっていくアフィン変換
// transform_animated: 回転適用後のアフィン変換
// src_mode:           描画するものが何なのか
// n_glyphs:           グリフの数
// gliphs:             グリフのデータ
// message:            任意入力メッセージ
// len_message:        現在のメッセージ長
// n_pixelbuffer:      ピクセルバッファのサイズ
typedef struct s_system {
	size_t		n_points;
	t_vector3d	*points_original;
	t_vector3d	*points_animated;

	t_affine	transform_static;
	t_affine	transform_animated;

	t_optics	optics;

	t_source	src_mode;
	size_t		n_glyphs;
	t_glyph		glyphs[T3_GLYPH_NUM];
	char		message[T3_MAX_MSGLEN + 1];
	size_t		len_message;

	size_t		n_pixelbuffer;
	char		pixelbuffer[T3_MAX_HEIGHT * (T3_MAX_WIDTH + 1)];
}	t_system;

char		**ft_rawsplit(char const *s, char c);
char		**ft_split(char const *s, char c);

char		**t3_read_all_lines(const char *file_path);
char		*rd_read_file_content(const char *filename);

void		t3_setup_system(t_system *system);
void		t3_check_tty_out(void);

void		t3_render_loop(t_system *system);

void		t3_init_render_params(t_system *system);

size_t		t3_count_lines(char **lines);
bool		t3_vectorize(const char *str, t_vector3d vector);
t_vector3d	*t3_read_vectors_from_file(const char *file_path);
void		t3_destroy_strarray(char **pstrs);

void		t3_affine_identity(t_affine a);
void		t3_affine_copy(t_affine dest, const t_affine src);
void		t3_affine_translate(
				t_affine dest,
				const t_affine src,
				const t_vector3d vec
				);
void		t3_affine_scale(
				t_affine dest,
				const t_affine src,
				const t_vector3d vec
				);
void		t3_affine_rot_axis(
				t_affine dest,
				const t_affine src,
				const t_vector3d n,
				double phi
				);
void		t3_apply_transform(t_vector3d v2, const t_vector3d v1, t_affine a);
void		t3_rot_vector_around_axis(t_vector3d r, t_vector3d n, double phi);
void		t3_affine_compose(
				t_affine dest, const t_affine src1, const t_affine src2);

t_ut		t3_get_ut(void);
t_ut		t3_wait_until(t_ut this_time);
void		t3_clear_pixelbuffer(t_system *system);
void		t3_fill_pixelbuffer(t_system *system);
void		t3_render(t_system *system);

void		t3_update_omega(t_system *system, int key);
void		t3_update_offset(t_system *system, int key);
void		t3_update_flame(t_system *system, int key);
void		t3_update_scale(t_system *system, int key);
void		t3_udpate_axis(t_system *system, int key);
void		t3_update_animate(t_system *system);
void		t3_reset_params(t_system *system);

bool		t3_scan_message(t_system *system);
void		t3_repoint(t_system *system);
void		t3_centralize_points(size_t n, t_vector3d *points);
int			t3_get_key(void);
void		t3_update_by_key(t_system *system);

size_t		t3_read_glyphs(t_glyph *glyphs);
int			t3_stdin_to_tty(void);
bool		t3_scan_message(t_system *sys);
void		t3_allocate_points(const char *str, t_system *system);

t_ut		t3_get_ut(void);
t_ut		t3_wait_until(t_ut this_time);

bool		t3_vectorize(const char *str, t_vector3d vector);

int			t3_max(int a, int b);
int			t3_min(int a, int b);

#endif
