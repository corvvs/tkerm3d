/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:58:31 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/16 14:21:43 by corvvs           ###   ########.fr       */
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
# include "libft.h"
# include <sys/time.h>
# include <fcntl.h>
# include <sys/select.h>

# define T3_WIDTH 160
# define T3_HEIGHT 60
typedef uint64_t	t_ut;

// 3次元ベクトル構造体
typedef double	t_vector3d[3];

// アフィン変換行列(4x4)
// a[16]でもいい？
typedef double	t_affine[4][4];

// カメラパラメータ構造体
typedef struct s_optics {
	// スクエアサイズ
	double		sq_size_x;
	double		sq_size_y;
	// オフセット
	double		offset_x;
	double		offset_y;
	// 位置(-offset_x, -offset_y)が原点(0, 0)に来るように移動してからレンダリングする
	// スケールファクター
	double		scale_factor;
	// 回転軸
	t_vector3d	rot_axis;

	// 回転角
	double		phi;
	// 角速度、つまり 1秒あたりの回転角度[ラジアン/マイクロ秒]
	double		omega;

	// 縦横文字数
	size_t		width;
	size_t		height;
	// ピクセルバッファ
	size_t		pixels[T3_HEIGHT][T3_WIDTH];
	double		fps;
	// uspf(マイクロ秒毎フレーム)
	t_ut		uspf;

	// 時間変化するかどうかのフラグ
	bool		animate;
} t_optics;

// 系全体を表す構造体
typedef struct s_system {
	// 点の数
	size_t		n_points;
	// 点のデータ
	t_vector3d	*points_original;
	// 回転後の点のデータ
	t_vector3d	*points_animated;
	// 2つのpointsはどちらも同サイズで、どちらもmalloc-ed.

	// 回転直前までもっていくアフィン変換
	t_affine	transform_static;
	// 回転適用後のアフィン変換
	t_affine	transform_animated;

	t_optics	optics;
} t_system;

char	*rd_read_file_content(const char *filename);

bool	rd_is_finite(const double val);
double	rd_nan(void);
size_t	t3_count_lines(char **lines);
bool	t3_vectorize(const char *str, t_vector3d vector);
t_vector3d	*t3_read_from_file(const char *file_path);
void	rd_free_strarray(char ***pstrs);

void	t3_affine_identity(t_affine a);
void	t3_affine_copy(t_affine dest, const t_affine src);
void	t3_affine_translate(t_affine dest, const t_affine src, const t_vector3d vec);
void	t3_affine_scale(t_affine dest, const t_affine src, const t_vector3d vec);
void	t3_affine_rot_axis(t_affine dest, const t_affine src, const t_vector3d n, double phi);
void	t3_apply_transform(t_vector3d v2, const t_vector3d v1, t_affine a);
void	t3_rot_vector_around_axis(t_vector3d r, t_vector3d n, double phi);

t_ut	t3_get_ut(void);
t_ut	t3_wait_until(t_ut this_time);
void	t3_clear_pixelbuffer(t_system *system);
void	t3_fill_pixelbuffer(t_system *system);
void	t3_render_pixel_buffer(t_system *system);

#endif
