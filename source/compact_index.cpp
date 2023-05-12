#include <hex/plugin.hpp>
#include <hex/helpers/fmt.hpp>
#include <hex/api/content_registry.hpp>

using namespace hex;
using namespace ContentRegistry;
using namespace ContentRegistry::DataInspector;

IMHEX_PLUGIN_SETUP("FCompactIndex", "Benjamin Moir", "Unreal Engine FCompactIndex type for the data inspector")
{
    DataInspector::add("FCompactIndex", 1, 5, [](auto buffer, auto endian, auto style)
    {
        std::string value;
        std::string format;
        u8 b0 = buffer[0];
        u32 v = b0 & 0x3F;
        int c = 1;

        if ((b0 & 0x40) != 0)
        {
            for (int i = 1, shift = 6; i < buffer.size() && shift < 28; i++, shift += 7)
            {
                c++;
                v |= (buffer[i] & 0x7F) << shift;

                if ((buffer[i] & 0x80) == 0) {
                    break;
                }
            }
        }

        switch (style)
        {
        case NumberDisplayStyle::Decimal:
            format = "{0}{1:d}";
            break;
        case NumberDisplayStyle::Hexadecimal:
            format = hex::format("{{0}}0x{{1:0{}X}}", (sizeof v) * 2);
            break;
        case NumberDisplayStyle::Octal:
            format = hex::format("{{0}}0o{{1:0{}o}}", (sizeof v) * 3);
            break;
        }

        value = hex::format(format, (b0 & 0x80) != 0 ? "-" : "", v);
        value = hex::format("{0} ({1} byte{2})", value, c, c > 1 ? "s" : "");
        return [value] { ImGui::TextUnformatted(value.c_str()); return value; };
    });
}
