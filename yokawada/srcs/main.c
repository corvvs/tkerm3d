/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corvvs <corvvs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 22:57:24 by corvvs            #+#    #+#             */
/*   Updated: 2022/02/15 21:30:13 by corvvs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term3d.h"

// 現在時刻をマイクロ秒単位で返す
t_ut	t3_get_ut(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec) * 1e6 + tv.tv_usec);
}

// this_time を超えるまで1000マイクロ秒ずつusleepする。
// その後、現在時刻を返す。
t_ut	t3_wait_until(t_ut this_time)
{
	t_ut	t1;

	while (true)
	{
		t1 = t3_get_ut();
		if (this_time <= t1)
		{
			break;
		}
		usleep(1000);
	}
	return (t1);
}

// doubleをビット操作するための共用体
typedef union u_ull_double
{
	unsigned long long	ull;
	double				dbl;
}	t_ull_double;

// アフィン変換を恒等変換に設定する
void	t3_affine_identity(t_affine a)
{
	int	i;

	i = 0;
	while (i < 4 * 4)
	{
		a[i / 4][i % 4] = (i / 4) == (i % 4);
		i += 1;
	}
}

// アフィン変換をコピーする
void	t3_affine_copy(t_affine dest, t_affine src)
{
	int	i;

	i = 0;
	while (i < 4 * 4)
	{
		dest[i / 4][i % 4] = src[i / 4][i % 4];
		i += 1;
	}
}

// アフィン変換 src にY軸まわりの回転(phi)を合成したものを dest にセットする
void	t3_affine_rot_y(t_affine dest, t_affine src, double phi)
{
	
	const double	r00 = +cos(phi) * src[0][0] - sin(phi) * src[2][0];
	const double	r02 = +cos(phi) * src[0][2] - sin(phi) * src[2][2];
	const double	r20 = +sin(phi) * src[0][0] + cos(phi) * src[2][0];
	const double	r22 = +sin(phi) * src[0][2] + cos(phi) * src[2][2];

	t3_affine_copy(dest, src);
	dest[0][0] = r00;
	dest[0][2] = r02;
	dest[2][0] = r20;
	dest[2][2] = r22;
}

// ベクトルv1に変換trを適用した結果をベクトルv2に格納する
void	t3_apply_transform(t_vector3d *v2, t_vector3d *v1, t_affine a)
{
	v2->x = a[0][0] * v1->x + a[0][1] * v1->y + a[0][2] * v1->z + a[0][3];
	v2->y = a[1][0] * v1->x + a[1][1] * v1->y + a[1][2] * v1->z + a[1][3];
	v2->z = a[2][0] * v1->x + a[2][1] * v1->y + a[2][2] * v1->z + a[2][3];
}

//　表示変換を適用する
void	t3_transform_to_render(t_system *system)
{
	size_t	i;

	i = 0;
	while (i < system->n_points)
	{
		t3_apply_transform(
			&system->points_animated[i],
			&system->points_original[i],
			system->transform_animated);
		i += 1;
	}
}

// 文字列配列を破壊する
void	rd_free_strarray(char ***pstrs)
{
	size_t	i;
	char	**strs;

	if (pstrs)
	{
		strs = *pstrs;
		if (strs)
		{
			i = 0;
			while (strs[i])
				free(strs[i++]);
			free(strs);
		}
		*pstrs = NULL;
	}
}
// 与えられたdoubleが有限かどうかを判定する
bool	rd_is_finite(const double val)
{
	t_ull_double		u;
	unsigned long long	fullbit;

	fullbit = (1ull << 11) - 1ull;
	u.dbl = val;
	return (((u.ull >> 52) & fullbit) != fullbit);
}

// nanを返す
double	rd_nan(void)
{
	t_ull_double	u;

	u.ull = (1ull << 63) - (1ull << 52) + 1;
	return (u.dbl);
}

// 文字列配列の長さを求める
size_t	t3_count_lines(char **lines)
{
	size_t	n;

	n = 0;
	while (lines[n])
	{
		n += 1;
	}
	return (n);
}

// カンマ区切りの文字列をベクトル(double,double,double)に変換する。失敗したらfalseを返す。
bool	t3_vectorize(const char *str, t_vector3d *vector)
{
	char			**splitted;

	splitted = ft_rawsplit(str, ',');
	if (!splitted)
		return (false);
	if (t3_count_lines(splitted) == 3)
	{
		vector->x = strtod(splitted[0], NULL);
		vector->y = strtod(splitted[1], NULL);
		vector->z = strtod(splitted[2], NULL);
		if (rd_is_finite(vector->x) && rd_is_finite(vector->y)
			&& rd_is_finite(vector->z))
		{
			rd_free_strarray(&splitted);
			return (true);
		}
	}
	rd_free_strarray(&splitted);
	return (false);
}

// file_path の中身を1つの文字列として読み取る
t_vector3d	*t3_read_from_file(const char *file_path)
{
	char		*content;
	char		**lines;
	t_vector3d	*points;
	size_t		n;
	size_t		i;
	
	content = rd_read_file_content(file_path);
	if (!content)
	{
		return (NULL);
	}
	lines = ft_split(content, '\n');
	free(content);
	if (!lines)
	{
		return (NULL);
	}
	n = t3_count_lines(lines);
	points = malloc(sizeof(t_vector3d) * (n + 1));
	if (!points)
	{
		return (NULL);
	}
	points[n].x = rd_nan();
	i = 0;
	double sx = 0;
	double sy = 0;
	double sz = 0;
	while (i < n)
	{
		if (!t3_vectorize(lines[i], &points[i]))
		{
			// しっぱい
			return (NULL);
		}
		sx += points[i].x;
		sy += points[i].y;
		sz += points[i].z;
		i += 1;
	}
	// printf("centroid: (%f, %f, %f)\n", sx / n, sy / n, sz / n);
	return (points);
}

// n個の位置ベクトルを、重心が(0, 0, 0)になるように平行移動する。
void	t3_centralize_points(size_t n, t_vector3d *points)
{
	size_t		i;
	t_vector3d	c;

	bzero(&c, sizeof(t_vector3d));
	i = 0;
	while (i < n)
	{
		c.x += points[i].x;
		c.y += points[i].y;
		c.z += points[i].z;
		i += 1;
	}
	c.x /= n;
	c.y /= n;
	c.z /= n;
	i = 0;
	while (i < n)
	{
		points[i].x -= c.x;
		points[i].y -= c.y;
		points[i].z -= c.z;
		i += 1;
	}
}

// opticsの設定
void	t3_init_optics(t_system *system)
{
	system->optics.width = T3_WIDTH;
	system->optics.height = T3_HEIGHT;
	system->optics.offset_x = 4;
	system->optics.offset_y = 4;
	system->optics.sq_size_x = (double)(+4 - -4) / T3_WIDTH;
	system->optics.sq_size_y = (double)(+4 - -4) / T3_HEIGHT;
	system->optics.phi = 0;
	// 2秒で半周するように角速度を設定
	system->optics.omega = M_PI * 2 / 1000000;
	// 1000000マイクロ秒(=1秒)をFPSで割ると、1フレームあたりのマイクロ秒数(uspf)が算出される。
	// これを使ってFPSをコントロールする(もっといい方法がありそう)。
	system->optics.uspf = 1000000 / T3_FPS;
}

// ピクセルバッファをクリア
void	t3_clear_pixelbuffer(t_system *system)
{
	size_t	i;

	i = 0;
	while (i < system->optics.height)
	{
		bzero(system->optics.pixels[i], sizeof(size_t) * system->optics.width);
		i += 1;
	}
}

// ピクセルバッファをセット
void	t3_fill_pixelbuffer(t_system *system)
{
	size_t	i;
	double	x;
	double	y;
	size_t	xi;
	size_t	yi;

	i = 0;
	while (i < system->n_points)
	{
		x = system->points_animated[i].x + system->optics.offset_x;
		y = system->points_animated[i].y + system->optics.offset_y;
		if (x >= 0 && y >= 0)
		{
			xi = (size_t)(x / system->optics.sq_size_x);
			yi = (size_t)(y / system->optics.sq_size_y);
			if (xi < system->optics.width && yi < system->optics.height)
			{
				system->optics.pixels[yi][xi] += 1;
			}
		}
		i += 1;
	}
}

// ピクセルバッファを表示
void	t3_render_pixel_buffer(t_system *system)
{
	char	buffer[T3_HEIGHT * (T3_WIDTH + 1)];
	char	nls[T3_HEIGHT];
	size_t	i;
	size_t	j;

	i = 0;
	while (i < system->optics.height)
	{
		j = 0;
		while (j < system->optics.width)
		{
			if (system->optics.pixels[i][j] > 3)
				buffer[i * (system->optics.width + 1) + j] = '#';
			else if (system->optics.pixels[i][j] > 1)
				buffer[i * (system->optics.width + 1) + j] = '*';
			else if (system->optics.pixels[i][j] > 0)
				buffer[i * (system->optics.width + 1) + j] = '.';
			else
				buffer[i * (system->optics.width + 1) + j] = ' ';
			j += 1;
		}
		buffer[i * (system->optics.width + 1) + j] = '\n';
		i += 1;
	}
	memset(nls, '\n', T3_HEIGHT);
	write(STDOUT_FILENO, nls, T3_HEIGHT);
	write(STDOUT_FILENO, buffer, T3_HEIGHT * (T3_WIDTH + 1));
}

int main(int argc, char **argv)
{
	t_system	system;

	if (argc != 2)
	{
		exit(1);
	}
	// [ファイルからのデータ読み取り]
	system.points_original = t3_read_from_file(argv[1]);
	if (!system.points_original)
	{
		exit(1);
	}
	system.n_points = 0;
	while (rd_is_finite(system.points_original[system.n_points].x))
	{
		system.n_points += 1;
	}

	// [処理用データ構造の初期化]
	system.points_animated = malloc(sizeof(t_vector3d) * (system.n_points + 1));
	t3_init_optics(&system);
	t3_centralize_points(system.n_points, system.points_original);
	t3_affine_identity(system.transform_static);

	// [描画ループ]
	t_ut	t0 = t3_get_ut();
	t_ut	t1;
	while (true)
	{
		t3_affine_rot_y(system.transform_animated, system.transform_static, system.optics.phi);
		t3_transform_to_render(&system);
		t3_clear_pixelbuffer(&system);
		t3_fill_pixelbuffer(&system);
		t1 = t3_wait_until(t0 + system.optics.uspf);
		t3_render_pixel_buffer(&system);
		// powとかsinがある係数は、回転速度をいい感じに揺らすためにかけている
		system.optics.phi += (0.1 + pow(sin(system.optics.phi), 2)) * system.optics.omega * (t1 - t0);
		t0 = t1;
	}
	
}
