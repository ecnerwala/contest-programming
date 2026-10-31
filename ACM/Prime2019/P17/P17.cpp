#include<bits/stdc++.h>
using namespace std;

inline int signum(int a) { return (a > 0) - (a < 0); }

pair<uint32_t, uint32_t> dp[2][2][3][3][2][2][3][3][2][2][3][3];
pair<uint32_t, uint32_t> ndp[2][2][3][3][2][2][3][3][2][2][3][3];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int XA, YA, ZA, XB, YB, ZB; cin >> XA >> YA >> ZA >> XB >> YB >> ZB;

	dp[1][1][0+1][0+1][1][1][0+1][0+1][1][1][0+1][0+1].first = 1;
	for (int l = 29; l >= 0; l--) {
		memset(ndp, 0, sizeof(ndp));
		for (int mv = 0; mv <= 1; mv++) {
			for (int xae = 0; xae < 2; xae ++) {
				for (int xav = 0; xav <= 1; xav ++) {
					if (xae && xav > ((XA>>l)&1)) {
						continue;
					}
					int nxae = xae && xav == ((XA>>l)&1);
					for (int xbe = 0; xbe < 2; xbe++) {
						for (int xbv = 0; xbv <= 1; xbv++) {
							if (xbe && xbv > ((XB>>l)&1)) {
								continue;
							}
							int nxbe = xbe && xbv == ((XB>>l)&1);
							for (int xd = -1; xd <= 1; xd++) {
								int nxd = xd ? xd : xav - xbv;
								for (int xm = -1; xm <= 1; xm++) {
									int nxm = xm * 2 + (xav - xbv) * nxd - mv;
									if (nxm > 1) continue;
									if (nxm < -1) nxm = -1;
									for (int yae = 0; yae < 2; yae ++) {
										for (int yav = 0; yav <= 1; yav ++) {
											if (yae && yav > ((YA>>l)&1)) {
												continue;
											}
											int nyae = yae && yav == ((YA>>l)&1);
											for (int ybe = 0; ybe < 2; ybe++) {
												for (int ybv = 0; ybv <= 1; ybv++) {
													if (ybe && ybv > ((YB>>l)&1)) {
														continue;
													}
													int nybe = ybe && ybv == ((YB>>l)&1);
													for (int yd = -1; yd <= 1; yd++) {
														int nyd = yd ? yd : yav - ybv;
														for (int ym = -1; ym <= 1; ym++) {
															int nym = ym * 2 + (yav - ybv) * nyd - mv;
															if (nym > 1) continue;
															if (nym < -1) nym = -1;
															for (int zae = 0; zae < 2; zae ++) {
																for (int zav = 0; zav <= 1; zav ++) {
																	if (zae && zav > ((ZA>>l)&1)) {
																		continue;
																	}
																	int nzae = zae && zav == ((ZA>>l)&1);
																	for (int zbe = 0; zbe < 2; zbe++) {
																		for (int zbv = 0; zbv <= 1; zbv++) {
																			if (zbe && zbv > ((ZB>>l)&1)) {
																				continue;
																			}
																			int nzbe = zbe && zbv == ((ZB>>l)&1);
																			for (int zd = -1; zd <= 1; zd++) {
																				int nzd = zd ? zd : zav - zbv;
																				for (int zm = -1; zm <= 1; zm++) {

																					int nzm = zm * 2 + (zav - zbv) * nzd - mv;
																					if (nzm > 1) continue;
																					if (nzm < -1) nzm = -1;

																					const auto& prv = dp[xae][xbe][xd+1][xm+1][yae][ybe][yd+1][ym+1][zae][zbe][zd+1][zm+1];
																					auto& nxt = ndp[nxae][nxbe][nxd+1][nxm+1][nyae][nybe][nyd+1][nym+1][nzae][nzbe][nzd+1][nzm+1];

																					int v = xav ^ yav ^ zav ^ xbv ^ ybv ^ zbv ^ mv;
																					nxt.first += prv.first;
																					nxt.second += prv.second;
																					nxt.second += prv.first * (uint32_t(v) << l);
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		memcpy(dp, ndp, sizeof(ndp));
	}

	uint32_t ans = 0;
	for (int xae = 0; xae < 2; xae ++) {
		for (int xbe = 0; xbe < 2; xbe++) {
			for (int xd = -1; xd <= 1; xd++) {
				for (int xm = -1; xm <= 1; xm++) {
					for (int yae = 0; yae < 2; yae ++) {
						for (int ybe = 0; ybe < 2; ybe++) {
							for (int yd = -1; yd <= 1; yd++) {
								for (int ym = -1; ym <= 1; ym++) {
									for (int zae = 0; zae < 2; zae ++) {
										for (int zbe = 0; zbe < 2; zbe++) {
											for (int zd = -1; zd <= 1; zd++) {
												for (int zm = -1; zm <= 1; zm++) {
													const auto& prv = dp[xae][xbe][xd+1][xm+1][yae][ybe][yd+1][ym+1][zae][zbe][zd+1][zm+1];
													if (max({xm,ym,zm}) == 0) ans += prv.second;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	ans &= (1u << 30)-1;
	cout << ans << '\n';
	return 0;
}
