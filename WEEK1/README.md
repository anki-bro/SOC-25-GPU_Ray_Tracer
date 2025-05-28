# ✅ Week 1: Vec3, Makefiles & Math Foundations

---

## ✅ Goals for Week 1

1. **Understand the math behind ray tracing**: vectors, dot product, cross product, reflection, refraction.
2. **Implement a `Vec3` class**.
3. **Learn Makefile basics** to compile and manage builds efficiently.
4. **Create your first image output** using a basic `.ppm` file writer (not using `Vec3` yet).

---

## 🛠️ Task Breakdown

### 1. Learn the Vector Math

Understand how vector math forms the foundation of ray tracing:

- How rays are defined
- How rays reflect and refract off surfaces
- Why normalization is important

### 📐 Vector Math & Ray Tracing

- [Ray Tracing Math – YouTube (The Cherno)](https://www.youtube.com/watch?v=4NshnkzOdI0)
- [Ray Tracing Math – YouTube (UC Davis Academics)](https://www.youtube.com/watch?v=Ahp6LDQnK4Y)

---

### 2. Implement a `Vec3` Class

Create a C++ class with the following:

- Constructor: `Vec3(float x, float y, float z)`
- Operator overloads: `+`, `-`, `*`, `/`, unary `-`, `[]`
- Methods:
  - `unit()`
  - `length()`
  - `dot(const Vec3& other)`
  - `cross(const Vec3& other)`
  - `reflect(const Vec3& normal)`
  - `refract(const Vec3& normal, float refractive_index)`

### 🧮 Vec3 & C++ Operator Overloading

- [Ray Tracing in One Weekend – Vec3 Chapter](https://raytracing.github.io/books/RayTracingInOneWeekend.html#vec3)
- [GeeksforGeeks – Operator Overloading in C++](https://www.geeksforgeeks.org/operator-overloading-c/)

🧪 **Testing**

- Use the provided `./test` executable to validate your `Vec3` class.

---

### 3. Makefile Basics

Write a `Makefile` that:

- Compiles your `vec3.h` and `main.cpp` into an executable.
- Supports:
  - `make`
  - `make clean`

### 🛠️ Makefile Basics

- [Make Tutorial](https://makefiletutorial.com/)
- [Makefile Tutorial for Beginners – YouTube (by Bro Code)](https://www.youtube.com/watch?v=_r7i5X0rXJk)
- [GNU Make Manual (Advanced Reference)](https://www.gnu.org/software/make/manual/make.html)

---

### 4. 🎨 Your First Rendered Image

Write a C++ program that outputs a `.ppm` file using simple color interpolation (e.g., gradient from top to bottom).
**Note:** You are _not_ using your `Vec3` class for this task.

Refer to the **“The PPM Image Format”** section from _[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage/theppmimageformat)_
