enum FactorizationMethod
{
	TrialDivision,
	FermatFactorization
}

service Tracker
{
	bool isPrime(1: i32 n);

	list<i32> factor(1: i32 n, 2: FactorizationMethod method);

	binary searchByPair(1: binary data);
}
