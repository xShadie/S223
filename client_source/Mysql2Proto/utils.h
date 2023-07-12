inline bool str_to_number (bool& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (strtol(in, NULL, 10) != 0);
	return true;
}

inline bool str_to_number (char& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (char) strtol(in, NULL, 10);
	return true;
}

inline bool str_to_number (unsigned char& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (unsigned char) strtoul(in, NULL, 10);
	return true;
}

inline bool str_to_number (short& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (short) strtol(in, NULL, 10);
	return true;
}

inline bool str_to_number (unsigned short& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (unsigned short) strtoul(in, NULL, 10);
	return true;
}

inline bool str_to_number (int& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (int) strtol(in, NULL, 10);
	return true;
}

inline bool str_to_number (unsigned int& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (unsigned int) strtoul(in, NULL, 10);
	return true;
}

inline bool str_to_number (long& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (long) strtol(in, NULL, 10);
	return true;
}

inline bool str_to_number (unsigned long& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (unsigned long) strtoul(in, NULL, 10);
	return true;
}

inline bool str_to_number (float& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (float) strtod(in, NULL);
	return true;
}

inline bool str_to_number (double& out, const char *in)
{
	if (0==in || 0==in[0])	return false;

	out = (double) strtod(in, NULL);
	return true;
}
