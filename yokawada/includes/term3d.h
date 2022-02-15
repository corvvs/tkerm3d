/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:58:31 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/15 10:52:45 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM3D_H
# define TERM3D_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <libft.h>

# define T3_WIDTH 40
# define T3_HEIGHT 20


// 3次元ベクトル構造体
typedef struct s_vector3d {
	double x;
	double y;
	double z;
} t_vector3d;

// アフィン変換行列(4x4)
// a[16]でもいい？
typedef double	t_affine[4][4];

// カメラパラメータ構造体
typedef struct s_optics {
	// スクエアサイズ
	double	sq_size_x;
	double	sq_size_y;
	// オフセット
	double	offset_x;
	double	offset_y;
	// 回転角
	double	phi;

	// 縦横文字数
	size_t	width;
	size_t	height;
	// ピクセルバッファ
	size_t	pixels[T3_HEIGHT][T3_WIDTH];
} t_optics;

// 系全体を表す構造体
typedef struct s_system {
	// 点の数
	size_t n_points;
	// 点のデータ
	t_vector3d *points_original;
  // 回転後の点のデータ
	t_vector3d *points_animated;
	// 2つのpointsはどちらも同サイズで、どちらもmalloc-ed.

	// 回転直前までもっていくアフィン変換
	t_affine transform_static;
	// 回転適用後のアフィン変換
	t_affine transform_animated;

	t_optics optics;
} t_system;

char	*rd_read_file_content(const char *filename);


#endif
