namespace cpp::common::inheritance {

class Integer {
   public:
    Integer(int i);
    Integer(Integer const&) = default;
    virtual ~Integer() = default;

    int mValue;
    char mFlag;
};

class NonPositiveInteger_V : public virtual Integer {
   public:
    NonPositiveInteger_V(int i);

    virtual ~NonPositiveInteger_V() = default;
};

class NonNegativeInteger_V : public virtual Integer {
   public:
    NonNegativeInteger_V(int i);

    virtual ~NonNegativeInteger_V() = default;
};

class NonPositiveInteger : public Integer {
   public:
    NonPositiveInteger(int i);

    virtual ~NonPositiveInteger() = default;
};

class NonNegativeInteger : public Integer {
   public:
    NonNegativeInteger(int i);

    virtual ~NonNegativeInteger() = default;
};

class ZeroInteger_V : public NonNegativeInteger_V, public NonPositiveInteger_V {
   public:
    ZeroInteger_V(int i);

    virtual ~ZeroInteger_V() = default;
};

class ZeroInteger : public NonNegativeInteger, public NonPositiveInteger {
   public:
    ZeroInteger(int i);

    virtual ~ZeroInteger() = default;
};

}  // namespace cpp::common::inheritance
