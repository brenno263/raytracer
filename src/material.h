#pragma once

class Material
{
public:
	virtual bool scatter(const Ray &in, const Collision &coll, Vec3 &attenuation, Ray &out) const = 0;
};

class Diffuse : public Material
{
public:
	Vec3 albedo;

	Diffuse(const Vec3 &a)
	{
		this->albedo = a;
	}

	bool scatter(const Ray &ray, const Collision &coll, Vec3 &attenuation, Ray &scattered) const override
	{
		Vec3 scatter_direction = coll.normal + Vec3::randomOnUnitSphere();

		//correct weirdness that might happen when randomOnUnitSphere ~= -normal
		if (scatter_direction.nearZero()) scatter_direction = coll.normal;

		scattered = Ray(coll.point, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class Specular : public Material
{
public:
	double fuzz;
	Vec3 albedo;

	Specular(Vec3 albedo, double fuzz)
	{
		this->albedo = albedo;
		this->fuzz = fuzz;
	}

protected:
	Vec3 reflect(const Vec3 &incoming, const Vec3 &normal) const
	{
		Vec3 reflect_direction = incoming - normal * 2 * incoming.dot(normal);

		return reflect_direction + (Vec3::randomOnUnitSphere() * fuzz);
	}

	bool scatter(const Ray &ray, const Collision &coll, Vec3 &attenuation, Ray &scattered) const override
	{
		scattered = Ray(coll.point, reflect(ray.direction(), coll.normal));
		attenuation = albedo;
		return true;
	}
};

class Dielectric : public Specular
{
public:
	double r_index;
	double r_index_ratio;

	Dielectric(Vec3 albedo, double r_index) : Specular(albedo, 0)
	{
		this->r_index = r_index;
		r_index_ratio = 1 / r_index;
	}

private:
	double schlick(double cos_theta) const
	{
		double t = (1 - r_index_ratio) / (1 + r_index_ratio);
		t = t * t;
		return t + (1 - t) * pow(1 - cos_theta, 5);
	}

	Vec3 refract(const Vec3 &incoming, const Vec3 &normal) const
	{
		double cos_theta = (incoming * -1).dot(normal);
		Vec3 outgoing_perp = (incoming + (normal * cos_theta)) * r_index_ratio;
		return outgoing_perp - (normal * sqrt(1.0 - outgoing_perp.dot(outgoing_perp)));
	}

public:
	bool scatter(const Ray &ray, const Collision &coll, Vec3 &attenuation, Ray &scattered) const override
	{
		double cos_theta = (ray.direction() * -1).dot(coll.normal);
		double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

		Vec3 direction;

		if (sin_theta * r_index_ratio > 1.0
			|| random_double() < schlick(cos_theta)) //reflect
		{
			direction = reflect(ray.direction(), coll.normal);
		} else
		{ //refract
			direction = refract(ray.direction(), coll.normal);
		}
		scattered = Ray(coll.point, direction);
		attenuation = albedo;
		return true;
	}
};
