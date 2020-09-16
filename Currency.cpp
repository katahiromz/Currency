// Currency.cpp
//////////////////////////////////////////////////////////////////////////////

#include "Currency.hpp"

namespace khmz
{

/*static*/ const UnsignedCurrency UnsignedCurrency::epsilon("0.000000000001");
/*static*/ const Currency Currency::epsilon("0.000000000001");

void UnsignedCurrency::unittest()
{
    // UnsignedCurrency::upow10
    assert(UnsignedCurrency::upow10(0) == 1);
    assert(UnsignedCurrency::upow10(1) == 10);
    assert(UnsignedCurrency::upow10(2) == 100);
    assert(UnsignedCurrency::upow10(3) == 1000);
    assert(UnsignedCurrency::upow10(4) == 10000);
    assert(UnsignedCurrency::upow10(5) == 100000);
    assert(UnsignedCurrency::upow10(6) == 1000000);
    assert(UnsignedCurrency::upow10(7) == 10000000);
    assert(UnsignedCurrency::upow10(8) == 100000000);
    assert(UnsignedCurrency::upow10(9) == 1000000000);
    assert(UnsignedCurrency::upow10(10) == 10000000000);
    assert(UnsignedCurrency::upow10(11) == 100000000000);
    assert(UnsignedCurrency::upow10(12) == 1000000000000);
    assert(UnsignedCurrency::upow10(13) == 10000000000000);
    assert(UnsignedCurrency::upow10(14) == 100000000000000);
    assert(UnsignedCurrency::upow10(15) == 1000000000000000);
    assert(UnsignedCurrency::upow10(16) == 10000000000000000);
    assert(UnsignedCurrency::upow10(17) == 100000000000000000);
    assert(UnsignedCurrency::upow10(18) == 1000000000000000000);
    assert(UnsignedCurrency::upow10(19) == std::numeric_limits<significand_t>::max());

    // ctor
    assert(UnsignedCurrency(0, 0) == "0");
    assert(UnsignedCurrency(1, 2) == "100");
    assert(UnsignedCurrency(10, -2) == "0.1");
    assert(UnsignedCurrency(10, -3) == "0.01");
    assert(UnsignedCurrency(12345, 0) == "12345");
    assert(UnsignedCurrency(12345, 10) == "123450000000000");
    assert(UnsignedCurrency(12345, 2) == "1234500");
    assert(UnsignedCurrency(12345, -2) == "123.45");
    assert(UnsignedCurrency(12345, -10) == "0.0000012345");

    UnsignedCurrency inf;
    inf.set_inf();
    assert(inf.to_string() == "inf");

    assert(UnsignedCurrency("1.1").to_string() == "1.1");
    assert(UnsignedCurrency("1.01").to_string() == "1.01");
    assert(UnsignedCurrency("1.001").to_string() == "1.001");
    assert(UnsignedCurrency("1.0001").to_string() == "1.0001");
    assert(UnsignedCurrency("1.00001").to_string() == "1.00001");
    assert(UnsignedCurrency("1.000001").to_string() == "1.000001");
    assert(UnsignedCurrency("1.0000001").to_string() == "1.0000001");
    assert(UnsignedCurrency("1.00000001").to_string() == "1.00000001");
    assert(UnsignedCurrency("1.000000001").to_string() == "1.000000001");
    assert(UnsignedCurrency("1.0000000001").to_string() == "1.0000000001");
    assert(UnsignedCurrency("1.00000000001").to_string() == "1.00000000001");
    assert(UnsignedCurrency("1.000000000001").to_string() == "1.000000000001");
    assert(UnsignedCurrency("1.0000000000001").to_string() == "1.0000000000001");
    assert(UnsignedCurrency("1.00000000000001").to_string() == "1.00000000000001");
    assert(UnsignedCurrency("1.000000000000001").to_string() == "1.000000000000001");
    assert(UnsignedCurrency("1.0000000000000001").to_string() == "1.0000000000000001");
    assert(UnsignedCurrency("1.00000000000000001").to_string() == "1.00000000000000001");
    assert(UnsignedCurrency("1.000000000000000001").to_string() == "1.000000000000000001");
    assert(UnsignedCurrency("1.0000000000000000001").to_string() == "1");
    assert(UnsignedCurrency("1.00000000000000000001").to_string() == "1");

    assert(UnsignedCurrency("9").to_string() == "9");
    assert(UnsignedCurrency("99").to_string() == "99");
    assert(UnsignedCurrency("999").to_string() == "999");
    assert(UnsignedCurrency("9999").to_string() == "9999");
    assert(UnsignedCurrency("99999").to_string() == "99999");
    assert(UnsignedCurrency("999999").to_string() == "999999");
    assert(UnsignedCurrency("9999999").to_string() == "9999999");
    assert(UnsignedCurrency("99999999").to_string() == "99999999");
    assert(UnsignedCurrency("999999999").to_string() == "999999999");
    assert(UnsignedCurrency("9999999999").to_string() == "9999999999");
    assert(UnsignedCurrency("99999999999").to_string() == "99999999999");
    assert(UnsignedCurrency("999999999999").to_string() == "999999999999");
    assert(UnsignedCurrency("9999999999999").to_string() == "9999999999999");
    assert(UnsignedCurrency("99999999999999").to_string() == "99999999999999");
    assert(UnsignedCurrency("999999999999999").to_string() == "999999999999999");
    assert(UnsignedCurrency("9999999999999999").to_string() == "9999999999999999");
    assert(UnsignedCurrency("99999999999999999").to_string() == "99999999999999999");
    assert(UnsignedCurrency("999999999999999999").to_string() == "999999999999999999");
    assert(UnsignedCurrency("9999999999999999999").to_string() == "inf");
    assert(UnsignedCurrency("99999999999999999999").to_string() == "inf");
    assert(UnsignedCurrency("999999999999999999999").to_string() == "inf");
    assert(UnsignedCurrency("9999999999999999999999").to_string() == "inf");

    assert(UnsignedCurrency("9.").to_string() == "9");
    assert(UnsignedCurrency("9.9").to_string() == "9.9");
    assert(UnsignedCurrency("9.99").to_string() == "9.99");
    assert(UnsignedCurrency("9.999").to_string() == "9.999");
    assert(UnsignedCurrency("9.9999").to_string() == "9.9999");
    assert(UnsignedCurrency("9.99999").to_string() == "9.99999");
    assert(UnsignedCurrency("9.999999").to_string() == "9.999999");
    assert(UnsignedCurrency("9.9999999").to_string() == "9.9999999");
    assert(UnsignedCurrency("9.99999999").to_string() == "9.99999999");
    assert(UnsignedCurrency("9.999999999").to_string() == "9.999999999");
    assert(UnsignedCurrency("9.9999999999").to_string() == "9.9999999999");
    assert(UnsignedCurrency("9.99999999999").to_string() == "9.99999999999");
    assert(UnsignedCurrency("9.999999999999").to_string() == "9.999999999999");
    assert(UnsignedCurrency("9.9999999999999").to_string() == "9.9999999999999");
    assert(UnsignedCurrency("9.99999999999999").to_string() == "9.99999999999999");
    assert(UnsignedCurrency("9.999999999999999").to_string() == "9.999999999999999");
    assert(UnsignedCurrency("9.9999999999999999").to_string() == "9.9999999999999999");
    // ---
    assert(UnsignedCurrency("9.99999999999999999").to_string() == "9.99999999999999999");
    assert(UnsignedCurrency("9.999999999999999999").to_string() == "9.99999999999999999");
    assert(UnsignedCurrency("9.9999999999999999999").to_string() == "9.99999999999999999");
    assert(UnsignedCurrency("9.99999999999999999999").to_string() == "9.99999999999999999");
    assert(UnsignedCurrency("9.999999999999999999999").to_string() == "9.99999999999999999");

    assert(UnsignedCurrency("0") == "0");
    assert(UnsignedCurrency("100") == "100");
    assert(UnsignedCurrency("0.1") == "0.1");
    assert(UnsignedCurrency("0.01") == "0.01");
    assert(UnsignedCurrency("12345") == "12345");
    assert(UnsignedCurrency("123450000000000") == "123450000000000");
    assert(UnsignedCurrency("1234500") == "1234500");
    assert(UnsignedCurrency("123.45") == "123.45");
    assert(UnsignedCurrency("0.0000012345") == "0.0000012345");

    // arith
    assert(UnsignedCurrency("1") + "1" == "2");
    assert(UnsignedCurrency("1") - "1" == "0");
    bool catched = false;
    try
    {
        UnsignedCurrency("1") - "2";
    }
    catch (const std::runtime_error&)
    {
        catched = true;
    }
    assert(catched);
    assert(UnsignedCurrency("2") - "1" == "1");
    assert(UnsignedCurrency("1.5") - "0.5" == "1");
    assert(UnsignedCurrency("1.5") * "2" == "3");

    assert(UnsignedCurrency("234.5") + "1.53456" == "236.03456");
    assert(UnsignedCurrency("1.53456") + "234.5" == "236.03456");
    assert(UnsignedCurrency("234.5") - "1.53456" == "232.96544");
    assert((UnsignedCurrency("9.9999999999") + "0.0000000001").to_string() == "10");
    catched = false;
    try
    {
        UnsignedCurrency("1.53456") - "234.5";
    }
    catch (const std::runtime_error&)
    {
        catched = true;
    }
    assert(catched);
    assert(UnsignedCurrency("234.5") * "1.53456" == "359.85432");
    assert(UnsignedCurrency("1.53456") * "234.5" == "359.85432");

    // comparison
    assert(UnsignedCurrency("1") != "10");
    assert(!(UnsignedCurrency("1") == "10"));
    assert(UnsignedCurrency("1") < "10");
    assert(!(UnsignedCurrency("1") > "10"));
    assert(UnsignedCurrency("1") <= "10");
    assert(!(UnsignedCurrency("1") >= "10"));

    assert(UnsignedCurrency(83, 4) < UnsignedCurrency(833, 3));
    assert(UnsignedCurrency(83, 4) <= UnsignedCurrency(833, 3));
    assert(UnsignedCurrency(833, 3) > UnsignedCurrency(83, 4));
    assert(UnsignedCurrency(833, 3) >= UnsignedCurrency(83, 4));

    assert(UnsignedCurrency(83, -3) < UnsignedCurrency(833, -4));
    assert(UnsignedCurrency(83, -3) <= UnsignedCurrency(833, -4));
    assert(UnsignedCurrency(833, -4) > UnsignedCurrency(83, -3));
    assert(UnsignedCurrency(833, -4) >= UnsignedCurrency(83, -3));

    puts("UnsignedCurrency::unittest: OK.");
}

void Currency::unittest()
{
    // ctor
    assert(Currency(0, 0) == "0");
    assert(Currency(1, 2) == "100");
    assert(Currency(10, -2) == "0.1");
    assert(Currency(10, -3) == "0.01");
    assert(Currency(12345, 0) == "12345");
    assert(Currency(12345, 10) == "123450000000000");
    assert(Currency(12345, 2) == "1234500");
    assert(Currency(12345, -2) == "+123.45");
    assert(Currency(-12345, -10) == "-0.0000012345");

    assert(Currency("0") == "0");
    assert(Currency("100") == "100");
    assert(Currency("0.1") == "0.1");
    assert(Currency("0.01") == "0.01");
    assert(Currency("12345") == "12345");
    assert(Currency("123450000000000") == "123450000000000");
    assert(Currency("1234500") == "1234500");
    assert(Currency("123.45") == "+123.45");
    assert(Currency("-0.0000012345") == "-0.0000012345");

    Currency inf;
    inf.set_inf();
    assert(inf.to_string() == "inf");
    inf.set_negative();
    assert(inf.to_string() == "-inf");

    // comparison
    assert(Currency("1") == "1");
    assert(!(Currency("1") != "1"));
    assert(Currency("-1") == "-1");
    assert(!(Currency("-1") != "-1"));
    assert(Currency("1") != "10");
    assert(Currency("1") != "-10");
    assert(Currency("-1") != "-10");
    assert(Currency("-1") != "10");
    assert(!(Currency("1") == "10"));
    assert(!(Currency("1") == "-10"));
    assert(!(Currency("-1") == "-10"));
    assert(!(Currency("-1") == "10"));
    assert(Currency("1") < "10");
    assert(!(Currency("1") < "-10"));
    assert(!(Currency("-1") < "-10"));
    assert(Currency("-1") < "10");
    assert(!(Currency("1") > "10"));
    assert(Currency("1") > "-10");
    assert(Currency("-1") > "-10");
    assert(!(Currency("-1") > "10"));
    assert(Currency("1") <= "10");
    assert(!(Currency("1") <= "-10"));
    assert(!(Currency("-1") <= "-10"));
    assert(Currency("-1") <= "10");
    assert(!(Currency("1") >= "10"));
    assert(Currency("1") >= "-10");
    assert(Currency("-1") >= "-10");
    assert(!(Currency("-1") >= "10"));

    // arith
    assert(Currency("1") + "1" == "2");
    assert(Currency("1") - "1" == "0");
    assert(Currency("1") - "2" == "-1");
    assert(Currency("2") - "1" == "1");
    assert(Currency("2") - "-1" == "3");
    assert(Currency("-2") - "1" == "-3");
    assert(Currency("-2") - "-1" == "-1");
    assert(Currency("-2") + "1" == "-1");
    assert(Currency("-2") + "-1" == "-3");
    assert(Currency("1.5") - "0.5" == "1");
    assert(Currency("1.5") + "-0.5" == "1");
    assert(Currency("1.5") * "2" == "3");
    assert(Currency("1.5") * "-2" == "-3");
    assert(Currency("-1.5") * "-2" == "3");

    assert(Currency("1.53456") + "234.5" == "236.03456");
    assert(Currency("1.53456") - "234.5" == "-232.96544");
    assert(Currency("1.53456") * "234.5" == "359.85432");

    assert(Currency("1.53456") + "-234.5" == "-232.96544");
    assert(Currency("1.53456") - "-234.5" == "236.03456");
    assert(Currency("1.53456") * "-234.5" == "-359.85432");

    assert(Currency("-1.53456") + "234.5" == "232.96544");
    assert(Currency("-1.53456") - "234.5" == "-236.03456");
    assert(Currency("-1.53456") * "234.5" == "-359.85432");

    assert(Currency("-1.53456") + "-234.5" == "-236.03456");
    assert(Currency("-1.53456") - "-234.5" == "232.96544");
    assert(Currency("-1.53456") * "-234.5" == "359.85432");

    assert(Currency("234.5") + "1.53456" == "236.03456");
    assert(Currency("234.5") - "1.53456" == "232.96544");
    assert(Currency("234.5") * "1.53456" == "359.85432");

    assert(Currency("234.5") + "-1.53456" == "232.96544");
    assert(Currency("234.5") - "-1.53456" == "236.03456");
    assert(Currency("234.5") * "-1.53456" == "-359.85432");

    assert(Currency("-234.5") + "1.53456" == "-232.96544");
    assert(Currency("-234.5") - "1.53456" == "-236.03456");
    assert(Currency("-234.5") * "1.53456" == "-359.85432");

    assert(Currency("-234.5") + "-1.53456" == "-236.03456");
    assert(Currency("-234.5") - "-1.53456" == "-232.96544");
    assert(Currency("-234.5") * "-1.53456" == "359.85432");

    assert(Currency(83, 4) < Currency(833, 3));
    assert(Currency(83, 4) <= Currency(833, 3));
    assert(Currency(833, 3) > Currency(83, 4));
    assert(Currency(833, 3) >= Currency(83, 4));

    assert(Currency(83, -3) < Currency(833, -4));
    assert(Currency(83, -3) <= Currency(833, -4));
    assert(Currency(833, -4) > Currency(83, -3));
    assert(Currency(833, -4) >= Currency(83, -3));

    puts("Currency::unittest: OK.");
}

} // namespace khmz

#ifdef UNITTEST
int main(void)
{
    using namespace khmz;
    UnsignedCurrency::unittest();
    Currency::unittest();
}
#endif
