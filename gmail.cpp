
// for convenience
#include <iostream>
#include <stdlib.h>
#include "lib.cpp"
#include <string>

int main()
{
    std::string access_token = "EwCIA8l6BAAUbDba3x2OMJElkF7gJ4z/VbCPEz0AAXiC7eRZmzq0gFccrnjUzDLRhur1iFdwideHW5LYgRkhigv+I0qBj/sa4QI/SEDTZ1OzZIBkANye1cJgJwLi3sdXzqNJGEYd+Iy8mSIAOzE71Kk6FHS2BWENapJbuUfXmelRddFmwaxW80rq16KhGRwQAdQrHHxq4VCe6SNngEf+rfbOxFJR+uxBVQimiikCZoJ8oUaARgRnV3JlMuol2ktJFzvPH3mBdYoXFNEtf81ZAQsVNl95Ijky/YlqyI/GBKZUkToaTlZNWzu5psByZpjKL5NiJ4kAhkB7Es/wILDKsGg7gbrOzrslSXxN6VTpBuNAkdO5SOEsLISmZxSiFQYQZgAAEDAx5nnE60E5fx3kjgY5EhlQArZi/YnL7celW/mXi5gjf/sCZrLfHy3V/iAtOus2n+eAtnV4rQq8QVbHKAKAZa9HMQB3FV6Sy2IqsLeMk0hb+Ej7qcuYfmlXDjxnNRvx8eEXsx67iilRIQIezFxm7SR1EHzqFEUsVwXCGmUojuuiN90mPMXxunm3zKkaEglSymx/7kIMGHsWHWgamGYNnUea6ZXqejSFWgFg0RBI3r0a5/Dn5LQEyWvg3ndXT9fkGt06+XkSAP2Kl+zKXvI9j3H9nSiD/N2eHNz0aBXbnr1RA+znR5bVmXwz89vlV3fhMZZgxcjHyCG4Be+eMQ6m94kdQ2WkPTTUV+BWhOglouqmCKwMKPNo6xMEVp22n5j9ytQofawI7q7mlRsHudj4VwN5rUH6gL9yFVf6tafo4sPtnDhzASYfKiLpRMfbm+B2Z428KYM3TthHqebZ9taoSCb5sFUc2S0DTzKXtm0ivaGlcneT8FUnD9kxxTCVFh9BGxT6ghku2SdaM1Rksvga/xKikHnPCrdVWLULBqiAA5VMavPH+WMjcsmbPp0MnkcgpmI2Ycgr2VcC7pHaiZV6oPa5u0SdMAMQUzFdO9/8NYgg5oQ/jq+AKhwaRxJM0u99np8kGDQbVnW4xi6Wql3j2vPtTR4Yp544B2ds8fkZiTJ5Kg0T9z8g1xTRi6rZeQTByFgrj7rWnnniK0zLsxRm7dx6SD7hrFqedwAE013EIpc3Ioi2RUvmWez0mzs/h+W9I3lFab8wWOXAoUS/efTYKMhCLbbpMxT7E+NCFl9ISMhWaOCIAg==";
    if (access_token == "")
    {
        std::cout << "Please provide your access token: " << std::endl;
        std::cin >> access_token;
    }

    GmailAPI g(access_token);
    // string res = g.fetch("https:/stackoverflow.com/");
    // cout << res << endl;

    return 0;
}
