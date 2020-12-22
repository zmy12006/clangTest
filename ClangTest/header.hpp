enum class Cpp11Enum
{
  RED = 10,
  BLUE = 20
};
#define TAG 123
struct Wowza
{
  virtual ~Wowza() = default;
  virtual void TAG foo(int i = 0) = 0;
};

struct Badabang : Wowza
{
  void foo(int) override;

  bool operator==(const Badabang& o) const;
};

template <typename T>
void bar(T&& t);